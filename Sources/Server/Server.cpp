/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:25:19 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/15 22:19:27 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

// Constructor

Server::Server(std::string const &pswd, int const &port) : _serverPswd(pswd), _serverStatus(false), _serverPort(port), _serverFd(-1), _epollFd(-1) {}

// Destructeur

Server::~Server() {
    closeFileDescriptors();
    std::cout << RED << "SERVER: Shut down." << RESET_COLOR << std::endl;
}

// Server Loop

extern int stopSignal;
void Server::serverLoop() {
    while (getServerStatus() && !stopSignal) {

        // Check which file descriptor is waiting for an event to occur    
        int numFdsEvents = epoll_wait(getEpollFd(), getEpollEventsArr(), CLIENT_NB, -1);

        for (int i = 0; i < numFdsEvents; ++i) {
            
            // Look for a new client connexion
            if (getEpollEventsArr()[i].data.fd == getServerFd()) {
                
                sockaddr_in newClientAddr;
                socklen_t newClientSize = sizeof(newClientAddr);
                
                int newClientFd = accept(getServerFd(), (struct sockaddr *)&newClientAddr, &newClientSize);
                if (newClientFd < 0) {
                    std::cerr << "SERVER: ERROR: Can't accept new client connection!" << std::endl;
                    std::cerr << strerror(errno) << std::endl;
                    continue;
                }

                // Solve new client's host name
                char host[NI_MAXHOST];
                char service[NI_MAXSERV];
                if (getnameinfo((struct sockaddr *)&newClientAddr, newClientSize, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV) != 0) {
                    close(newClientFd);
                    continue;
                }

                // Add new client to epoll
                getEpollEvent().events = EPOLLIN;
                getEpollEvent().data.fd = newClientFd;

                if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, newClientFd, &getEpollEvent()) < 0) {
                    std::cerr << RED << "SERVER: ERROR: Can't add client to epoll!" << RESET_COLOR << std::endl;
                    std::cerr << RED << strerror(errno) << RESET_COLOR << std::endl;
                    close(newClientFd);
                }

                std::cout << GREEN << "SERVER: New client successfully added!" << RESET_COLOR << std::endl;
                
                // Register new client
                
                this->_clients[newClientFd] = new Client(newClientFd, std::string(host));
            }
            else {
				char    buffer[1024] = {0};
				int     user_fd = this->_epollEventsArr[i].data.fd;
				int     bytes_received = recv(user_fd, buffer, sizeof(buffer) - 1, 0);

				if (bytes_received > 0)
				{
					std::cout << YELLOW << "[" << user_fd << "]: " << RESET_COLOR << buffer << std::endl;
                    std::cout << buffer << std::endl;
				}
				else if (bytes_received == 0)
				{
                    epoll_ctl(getEpollFd(), EPOLL_CTL_DEL, user_fd, NULL);
					delete this->_clients[user_fd];
                    this->_clients.erase(user_fd);
                    close(user_fd);
                    std::cout << RED << "[" << user_fd << "]: disconnected. " << RESET_COLOR << buffer << std::endl;                    
				}
                else {
                    std::cerr << RED << "SERVER: ERROR: recv failed for client " << user_fd << RESET_COLOR << std::endl;
                    std::cerr << RED << strerror(errno) << RESET_COLOR << std::endl;
                    epoll_ctl(getEpollFd(), EPOLL_CTL_DEL, user_fd, NULL);
                    close(user_fd);
                    delete this->_clients[user_fd];
                    this->_clients.erase(user_fd);
                }
            }
        }
    }
    setServerStatus(false);
}

// Server's cleaner

void Server::closeFileDescriptors() {
    if (getServerFd() >= 0) {
        close(getServerFd());
        setServerFd(-1);
    }
    if (getEpollFd() >= 0) {
        close(getEpollFd());
        setEpollFd(-1);
    }
    setServerStatus(false);
    std::cerr << YELLOW << "SERVER: Cleanup completed." << RESET_COLOR << std::endl;
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

int Server::getServerFd() const {
    return (_serverFd);
}

int Server::getServerPort() const {
    return (_serverPort);
}

int Server::getEpollFd() const {
    return (_epollFd);
}

bool Server::getServerStatus() const {
    return (_serverStatus);
}

struct epoll_event &Server::getEpollEvent() {
    return (_epollEvent);
}

struct epoll_event *Server::getEpollEventsArr() {
    return (_epollEventsArr);
}