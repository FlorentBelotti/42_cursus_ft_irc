/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:25:19 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/01 19:03:33 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

// Animation management

void    *Server::serverIsRunningAnimation(void *arg) {
    
    Server* server = static_cast<Server*>(arg);
    
    std::cout << YELLOW << "Server: Is currently running";
    std::cout.flush();
    while (server->getServerStatus() == true) {
        for (int i = 0; i < 3; i++) {
            usleep(500000);
            std::cout << ".";
            std::cout.flush();
        }
        if (server->getServerStatus() == true) {
            usleep(500000);
            std::cout << "\b\b\b   \b\b\b";
            std::cout.flush();
        }
    }
    std::cout << std::endl;
    return NULL;
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
    std::cerr << YELLOW << "Server: Cleanup completed." << RESET_COLOR << std::endl;
}

// Constructor

Server::Server(std::string const &pswd, int const &port) : _serverPswd(pswd), _serverStatus(false), _serverPort(port), _serverFd(-1), _epollFd(-1) {}

// Destructeur

Server::~Server() {
    closeFileDescriptors();
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