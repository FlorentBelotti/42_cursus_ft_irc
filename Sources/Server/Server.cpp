/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:25:19 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/01 00:45:38 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

// Animation management

void    *Server::serverIsRunningAnimation(void *arg) {
    
    Server* server = static_cast<Server*>(arg);
    
    std::cout << YELLOW << "Server: Is currently running";
    std::cout.flush();
    while (server->getServerStatus() == false) {
        for (int i = 0; i < 3; i++) {
            usleep(500000);
            std::cout << ".";
            std::cout.flush();
        }
        if (server->getServerStatus() == false) {
            usleep(500000);
            std::cout << "\b\b\b   \b\b\b";
            std::cout.flush();
        }
    }
    std::cout << std::endl;
    std::cout << GREEN << "Server: Successfully created!" << std::endl;
    return NULL;
}

// Socket(s) management

bool Server::bindSocketToAdress() {
    
    // Instance of of sockaddr_in called _serverAdress
    
    _serverAdress.sin_family = AF_INET;
    _serverAdress.sin_port = htons(getServerPort());
    _serverAdress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    
    if (bind(getServerFd(), (struct sockaddr *)&_serverAdress, sizeof(_serverAdress)) < 0) {
        std::cerr << RED << "Error: Cannot bind the socket!" << RESET_COLOR << std::endl;
        std::cerr << strerror(errno) << std::endl;
        close(getServerFd());
        return false;
    }
    return true;
}

bool Server::setSocketNonBlockingMode() {
    
    // Remove previous flag from server's socket
    
    int sockFlags = fcntl(getServerFd(), F_GETFL, 0);
    if (sockFlags == -1) {
        std::cerr << RED << "Server: Error: Cannot get server socket's flag!" << RESET_COLOR << std::endl;
        std::cerr << strerror(errno) << std::endl;
        return false;
    }

    // Add non-block flag to server's socket
    
    if (fcntl(getServerFd(), F_SETFL, sockFlags | O_NONBLOCK)) {
        std::cerr << RED << "Server: Error: Cannot set server's socket to non-block!" << RESET_COLOR << std::endl;
        std::cerr << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

// Epoll and events

bool    Server::manageEpollAndEvents() {
    
    // Create epoll instance 
    
    setEpollFd(epoll_create1(0));
    if (getEpollFd() < 0) {
        std::cerr << RED << "Server: Error: Cannot create epoll!" << RESET_COLOR << std::endl;
        std::cerr << strerror(errno) << std::endl;
        close(getServerFd());
        return false;
    }
    
    // Configure epoll instance

    this->_epollEvent.events = EPOLLIN;
    this->_epollEvent.data.fd = getServerFd();

    // Adding server's socket to epoll instance

    if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, getServerFd(), &(this->_epollEvent)) < 0)
	{
		std::cerr << "Error: Cannot add socket to epoll!" << std::endl;
		std::cerr << strerror(errno) << std::endl;
        close(getServerFd());
        close(getEpollFd());
        return false;
	}
    return true;
}

bool Server::createSocket() {
    setServerFd(socket(AF_INET, SOCK_STREAM, 0));
    if (getServerFd() < 0) {
        std::cerr << RED << "Server: Error: Cannot create the server socket!" << RESET_COLOR << std::endl;
        std::cerr << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool Server::setSocketOptions() {
	int options = 1;
	setsockopt(getServerFd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(options));
	if (listen(getServerFd(), 10) < 0) {
        std::cerr << RED << "Server: Error: Cannot listen on the socket!" << RESET_COLOR << std::endl;
        std::cerr << strerror(errno) << std::endl;
        close(getServerFd());
        return false;
    }
    return true;
}

void Server::setupSocketAndEvents() {
    
    bool    flag;

    flag = createSocket();
    flag = setSocketNonBlockingMode();
    flag = bindSocketToAdress();
    flag = setSocketOptions();
    flag = manageEpollAndEvents();
    if (!flag) {
        setServerStatus(false);
    }
    setServerStatus(true);
    std::cout << GREEN << "Server: successfully created!" << std::endl;
}

// Constructor

Server::Server(std::string const &pswd, int const &port) : _serverPswd(pswd), _serverPort(port) {}

// Destructeur

Server::~Server() {
    std::cout << RED << "Server: Shut down." << RESET_COLOR << std::endl;
}

// Setters

void Server::setServerStatus(bool status) {
    _serverStatus = status;
}

void Server::setServerFd(int serverFd) {
    _serverFd = serverFd;
}

void Server::setEpollFd(int epollFd) {
    _epollFd = epollFd;
}

// Getters

int Server::getServerFd() {
    return (_serverFd);
}

int Server::getServerPort() {
    return (_serverPort);
}

int Server::getEpollFd() {
    return (_epollFd);
}

bool Server::getServerStatus() {
    return (_serverStatus);
}

struct epoll_event &Server::getEpollEvent() {
    return (_epollEvent);
}