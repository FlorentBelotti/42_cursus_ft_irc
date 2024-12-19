/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:21:26 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/19 12:36:05 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Client.hpp"

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

bool Client::getOperatorStatus() const {
    return _isOperator;
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

void Client::setClientBuffer(std::string msg) {
    _buffer += msg;
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

void Client::setClientOperatorStatus(bool status) {
    _isOperator = status;
}

void Client::setClientChannel(std::string channelName, Channel *channel) {
    _clientChannels[channelName] = channel;
}

void Client::removeClientChannel(std::string channelName) {
    _clientChannels.erase(channelName);
}

// Commands

bool Client::isValidName(const std::string& name) {
    if (name.length() > 16) {
        return false;
    }
    for (size_t i = 0; i < name.length(); ++i) {
        if (!isalnum(name[i]) && name[i] != '_' && name[i] != '-') {
            return false;
        }
    }
    return true;
}

void Client::clientNickname(const std::string &args) {
    
    if (args.empty() || args.find(' ') != std::string::npos) {
        std::string errorMsg = "USAGE : /nickname <nickname>";
        sendMessage(errorMsg);
        return;
    }

    if (!isValidName(args)) {
        std::string errorMsg = "USAGE: Nickname must be 16 characters max and contain only letters, digits, '_', and '-'.";
        sendMessage(errorMsg);
        return;
    }

    setClientNickname(args);
    std::string successMsg = "SERVER: Nickname changed to " + args;
    sendMessage(successMsg);
}

// Methods

void Client::sendMessage(std::string const &msg) {
    std::cout << YELLOW << "Sending message to client: " << msg << RESET_COLOR << std::endl;
}irc

void Client::sendFile(std::string const &file) {
    (void)file;
}

void Client::sendInvite(Client const *client) {
    (void)client;
}