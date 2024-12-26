/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:25:19 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/26 21:41:54 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"

// Constructor

Server::Server(std::string const &pswd, int const &port) : _serverPswd(pswd), _serverStatus(false), _serverPort(port), _serverFd(-1), _epollFd(-1) {}

// Destructeur

Server::~Server() {
    std::cout << RED << "[SERVER]: Shut down." << RESET_COLOR << std::endl;
}

// Server Loop

void Server::handleNewClient() {
    sockaddr_in newClientAddr;
    socklen_t newClientSize = sizeof(newClientAddr);

    int newClientFd = accept(getServerFd(), (struct sockaddr *)&newClientAddr, &newClientSize);
    if (newClientFd < 0) {
        std::cerr << "[SERVER]: [ERROR]: Can't accept new client connection!" << std::endl;
        std::cerr << strerror(errno) << std::endl;
        return;
    }

    // Define hostname
    
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    if (getnameinfo((struct sockaddr *)&newClientAddr, newClientSize, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV) != 0) {
        close(newClientFd);
        return;
    }

    // Add client to epoll
    
    getEpollEvent().events = EPOLLIN;
    getEpollEvent().data.fd = newClientFd;
    if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, newClientFd, &getEpollEvent()) < 0) {
        std::cerr << RED << "[SERVER]: [ERROR]: Can't add client to epoll!" << RESET_COLOR << std::endl;
        std::cerr << RED << strerror(errno) << RESET_COLOR << std::endl;
        close(newClientFd);
        return;
    }

    std::cout << GREEN << "[SERVER]: New client successfully added!" << RESET_COLOR << std::endl;

    // Save new client
    
    getClients()[newClientFd] = new Client(newClientFd, std::string(host));
}

std::vector<std::string> Server::splitCommands(const std::string& message) {
    std::vector<std::string> commands;
    std::istringstream stream(message);
    std::string command;
    while (std::getline(stream, command, '\n')) {
        if (!command.empty() && command[command.size() - 1] == '\r') {
            command.erase(command.size() - 1); // Remove the trailing '\r'
        }
        commands.push_back(command);
    }
    return commands;
}

void Server::handleClientEvent(int user_fd) {
    
    char buffer[1024] = {0};
    int bytes_received = recv(user_fd, buffer, sizeof(buffer) - 1, 0);

    // Client message

    if (bytes_received > 0) {
        std::string message(buffer, bytes_received);
        std::vector<std::string> commands = splitCommands(message);
        for (size_t i = 0; i < commands.size(); ++i) {
            handleMessage(commands[i], user_fd);
        }
    } 
    
    // Client disconnected
    
    else {
        epoll_ctl(getEpollFd(), EPOLL_CTL_DEL, user_fd, NULL);
        std::cout << RED << "[" << getClients()[user_fd]->getClientHostname() << "]: Disconnected." << RESET_COLOR << std::endl;
        delete getClients()[user_fd];
        getClients().erase(user_fd);
        close(user_fd);
    }
}

extern int stopSignal;
void Server::serverLoop() {
    while (getServerStatus() && !stopSignal) {
        
        // Listen for events
        
        int numFdsEvents = epoll_wait(getEpollFd(), getEpollEventsArr(), CLIENT_NB, -1);

        for (int i = 0; i < numFdsEvents; ++i) {

            // New client connection
            
            if (getEpollEventsArr()[i].data.fd == getServerFd()) {
                handleNewClient();
            } 
            
            // Client event
            
            else {
                int user_fd = getEpollEventsArr()[i].data.fd;
                handleClientEvent(user_fd);
            }
        }
    }
    clearClients();
    closeFileDescriptors();
}

// Server's cleaner

void Server::clearClients() {
    for (std::map<int, Client *>::iterator it = getClients().begin(); it != getClients().end(); ++it)
    {
        close(it->first);
        delete it->second;
    }
}

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
    std::cerr << YELLOW << "[SERVER]: Cleanup completed." << RESET_COLOR << std::endl;
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

// Channel

void Server::addServerChannel(std::string channelName, Channel *channel) {
    _channels[channelName] = channel;
}

void Server::removeServerChannel(std::string channelName) {
    delete _channels[channelName];
    _channels.erase(channelName);
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

std::map<int, Client *> &Server::getClients() {
    return (_clients);
}

std::map<std::string, Channel *> &Server::getServerChannels() {
    return (_channels);
}

std::string Server::getServerPswd() const {
    return (_serverPswd);
}

Client *Server::getClientByNickname(std::string const &nickname) {
    for (std::map<int, Client *>::iterator it = getClients().begin(); it != getClients().end(); ++it) {
        if (it->second->getClientNickname() == nickname) {
            return it->second;
        }
    }
    return NULL;
}