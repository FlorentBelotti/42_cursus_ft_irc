/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:21:26 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/15 22:11:17 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"

// Constructor

Client::Client(int newClientFd, std::string hostname) : _isLogged(false), _isBot(false), _pswdTries(0), _clientFd(newClientFd), _hostname(hostname) {}

// Destructor

Client::~Client() {}

// Getters
        
int Client::getClientFd() const {
    return _clientFd;
}

std::string Client::getClientHostname() const {
    return _hostname;
}

std::string Client::getClientNickname() const {
    return _nickname;
}

std::string Client::getClientUsername() const {
    return _username;
}

std::string Client::getClientBuffer() const {
    return _buffer;
}

bool Client::getClientBotStatus() const {
    return _isBot;
}

bool Client::getClientLogStatus() const {
    return _isLogged;
}

int Client::getClientPswdTries() const {
    return _pswdTries;
}
        
// Setters
    
void Client::setClientFd(int clientFd) {
    _clientFd = clientFd;
}

void Client::setClientIp(std::string clientIp) {
    _clientIp = clientIp;
}

void Client::setClientHostname(std::string hostname) {
    _hostname = hostname;
}

void Client::setClientNickname(std::string nickname) {
    _nickname = nickname;
}

void Client::setClientUsername(std::string username) {
    _username = username;
}

void Client::setClientBuffer(std::string buffer) {
    _buffer = buffer;
}

void Client::setClientBotStatus(bool status) {
    _isBot = status;
}

void Client::setClientLogStatus(bool status) {
    _isLogged = status;
}

void Client::setClientPswdTries(int tries) {
    _pswdTries = tries;
}
