/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 18:47:06 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/28 23:51:58 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

// Socket(s) management

bool Server::bindSocketToAddress() {
    
    // Instance of of sockaddr_in called _serverAdress
    
    _serverAdress.sin_family = AF_INET;
    _serverAdress.sin_port = htons(getServerPort());
    _serverAdress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    
    if (bind(getServerFd(), (struct sockaddr *)&_serverAdress, sizeof(_serverAdress)) < 0)
        return false;
    return true;
}

bool Server::setSocketNonBlockingMode() {
    
    // Remove previous flag from server's socket
    
    // int sockFlags = fcntl(getServerFd(), F_GETFL, 0);
    // if (sockFlags == -1) {
    //     std::cerr << RED << "[SERVER]: [ERROR]: Cannot get server socket's flag!" << RESET_COLOR << std::endl;
    //     std::cerr << strerror(errno) << std::endl;
    //     return false;
    // }

    // Add non-block flag to server's socket
    
    if (fcntl(getServerFd(), F_SETFL, O_NONBLOCK))
        return false;
    return true;
}

// Epoll and events

bool    Server::manageEpollAndEvents() {
    
    // Create epoll instance 
    
    setEpollFd(epoll_create1(0));
    if (getEpollFd() < 0)
        return false;
    
    // Configure epoll instance

    this->_epollEvent.events = EPOLLIN;
    this->_epollEvent.data.fd = getServerFd();

    // Adding server's socket to epoll instance

    if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, getServerFd(), &(this->_epollEvent)) < 0)
        return false;
    return true;
}

bool Server::createSocket() {
    setServerFd(socket(AF_INET, SOCK_STREAM, 0));
    if (getServerFd() < 0)
        return false;
    return true;
}

bool Server::setSocketOptions() {
	int options = 1;
	setsockopt(getServerFd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(options));
	if (listen(getServerFd(), CLIENT_NB) < 0)
        return false;
    return true;
}

// Error management

void Server::manageSocketError(int step) {
    std::string errorMsg;
    switch (step) {
        case CREATE_SOCKET:
            errorMsg = "[SERVER]: [ERROR]: Cannot create the server socket!";
            break;
        case SET_NONBLOCKING:
            errorMsg = "[SERVER]: [ERROR]: Cannot set server's socket to non-block!";
            break;
        case BIND_SOCKET:
            errorMsg = "[SERVER]: [ERROR]: Cannot bind the socket!";
            break;
        case SET_OPTIONS:
            errorMsg = "[SERVER]: [ERROR]: Cannot set socket options!";
            break;
        case MANAGE_EPOLL:
            errorMsg = "[SERVER]: [ERROR]: Cannot manage epoll and events!";
            break;
    }
    std::cerr << RED << errorMsg << RESET_COLOR << std::endl;
    std::cerr << strerror(errno) << std::endl;
    closeFileDescriptors();
    setServerStatus(false);
}

void Server::setupSocketAndEvents() {
    bool flag;

    for (int step = CREATE_SOCKET; step <= MANAGE_EPOLL; ++step) {
        switch (step) {
            case CREATE_SOCKET:
                flag = createSocket();
                break;
            case SET_NONBLOCKING:
                flag = setSocketNonBlockingMode();
                break;
            case BIND_SOCKET:
                flag = bindSocketToAddress();
                break;
            case SET_OPTIONS:
                flag = setSocketOptions();
                break;
            case MANAGE_EPOLL:
                flag = manageEpollAndEvents();
                break;
        }
        if (!flag) {
            manageSocketError(step);
            return;
        }
    }
    setServerStatus(true);
    std::cout << GREEN << "[SERVER]: Successfully created!" << RESET_COLOR << std::endl;
}