/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:25:19 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/12 22:16:27 by fbelotti         ###   ########.fr       */
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

void Server::serverLoop() {
    while (getServerStatus()) {

        // Check which file descriptor is waiting for an event to occur    
    
        int numFdsEvents = epoll_wait(getEpollFd(), getEpollEventsArr(), CLIENT_NB, -1);

        for (int i = 0; i < numFdsEvents; ++i) {
            
            // Look for a new client connexion

            if (getEpollEventsArr()[i].data.fd == getServerFd()) {
                
                sockaddr_in newClientAddr;
                socklen_t   newClientSize = sizeof(newClientAddr);
                
                int newClientFd = accept(getServerFd(), (struct sockaddr *)&newClientAddr, &newClientSize);
                if (newClientFd < 0) {
                    std::cerr << "SERVER: ERROR: Can't accept new client connection!" << std::endl;
                    std::cerr << strerror(errno) << std::endl;
                    continue;
                }

            // Solve new client's host name

                // Usefull flag (Dashboard) :
                    // NI_NOFQDN: only local host name,
                    // NI_NUMERICHOST: numeric adress,
                    // NI_NAMEREQD: failure in case of unindentified host,
                    // NI_NUMERICSERV: only port number.

            char host[NI_MAXHOST];
            char service[NI_MAXSERV];
            if (getnameinfo((struct sockaddr *)&newClientAddr, newClientSize, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV) != 0) {
                    close(newClientFd);
                    continue;
                }

            // Add new client to epoll

            getEpollEvent().events = EPOLLIN;
            getEpollEvent().data.fd = newClientFd;

            	if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, newClientFd, &getEpollEvent()) < 0)
				{
					std::cerr << RED << "SERVER: ERROR: Can't add client to epoll!" << RESET_COLOR << std::endl;
					std::cerr << RED << strerror(errno) << RESET_COLOR << std::endl;
					close(newClientFd);
				}

				std::cout << GREEN << "SERVER: New client successfully added!" << RESET_COLOR << std::endl;
				
                // Client class to setup.
                // this->clients[newsockfd] = new IrcClient(newsockfd, std::string(host));
            }
            else {
                // To continue.
            }
        }
    }
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