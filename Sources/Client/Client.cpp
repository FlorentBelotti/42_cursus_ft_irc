/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:21:26 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/20 01:14:17 by fbelotti         ###   ########.fr       */
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

std::string Client::getClientServername() const {
    return _servername;
}

std::string Client::getClientRealname() const {
    return _realname;
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

void Client::setClientServername(std::string servername) {
    _servername = servername;
}

void Client::setClientRealname(std::string realname) {
    _realname = realname;
}

// Commands

bool Client::isValidName(const std::string& name, size_t max_length) {
    if (name.length() > max_length || name.length() < 1) {
        return false;
    }
    for (size_t i = 0; i < name.length(); ++i) {
        if (!isalnum(name[i]) && name[i] != '_' && name[i] != '-') {
            return false;
        }
    }
    return true;
}

std::vector<std::string> Client::getArgsVector(const std::string &args) {
    std::vector<std::string> argsVector;
    std::istringstream iss(args);
    std::string token;
    while (iss >> token) {
        argsVector.push_back(token);
    }
    return argsVector;
}

void Client::clientNicknameCommand(const std::string &args) {
    
    if (args.empty() || args.find(' ') != std::string::npos) {
        std::string errorMsg = "[USAGE]: /nickname <nickname>";
        sendMessage(errorMsg, RED);
        return;
    }

    if (!isValidName(args, 9)) {
        std::string errorMsg = "[USAGE]: Nickname must be between 1 and 9 characters and contain only letters, digits, '_', and '-'.";
        sendMessage(errorMsg, RED);
        return;
    }

    setClientNickname(args);
    std::string successMsg = "[COMMAND]: Nickname changed to " + getClientNickname();
    sendMessage(successMsg, GREEN);
}

void Client::clientUserCommand(const std::string &args) {
    
    std::vector<std::string> argsVector = getArgsVector(args);
    
    if (argsVector.size() != 4) {
        std::string errorMsg = "[USAGE]: /username <username> <hostname> <servername> <realname>";
        sendMessage(errorMsg, RED);
        return;
    }

    for (size_t i = 0; i < argsVector.size(); ++i) {
        if (!isValidName(argsVector[i], 16)) {
            std::string errorMsg = "[USAGE]: Each argument must be 16 characters max and contain only letters, digits, '_', and '-'.";
            sendMessage(errorMsg, RED);
            return;
        }
    }

    setClientUsername(argsVector[0]);
    setClientHostname(argsVector[1]);
    setClientServername(argsVector[2]);
    setClientRealname(argsVector[3]);
    std::string successMsg = "[COMMAND]: Client infos updated\n";
    successMsg += "Username: " + getClientUsername() + "\n";
    successMsg += "Hostname: " + getClientHostname() + "\n";
    successMsg += "Servername: " + getClientServername() + "\n";
    successMsg += "Realname: " + getClientRealname() + "\n";
    sendMessage(successMsg, GREEN);
}

void Client::clientJoinCommand(const std::string &args, Server *server) {
    
    // Check if channel exists
    
    if (server->getServerChannels().find(args) != server->getServerChannels().end()) {

        // Client join channel
        
        setClientChannel(args, server->getServerChannels()[args]);
        server->getServerChannels()[args]->addClient(this);
        std::string successMsg = "[COMMAND]: Channel " + args + " joined.";
        sendMessage(successMsg, GREEN);
    } 
    
    
    else {
    
        // Check if channel name is valid
        
        if (args[0] != '#' && args[0] != '&') {
            std::string errorMsg = "[USAGE]: Channel name must start with a '#' or '&' character.";
            sendMessage(errorMsg, RED);
            return;
        }
        if (!isValidName(&args[1], 9)) {
            std::string errorMsg = "[USAGE]: Channel name must be between 1 and 9 characters and contain only letters, digits, '_', and '-'.";
            sendMessage(errorMsg, RED);
            return;
        }

        // Create channel
        
        server->setServerChannel(args, new Channel(args));
        
        // Add channel to client
        
        setClientChannel(args, server->getServerChannels()[args]);

        // Add client to channel
        
        server->getServerChannels()[args]->addClient(this);

        // Add client to channel's operators

        server->getServerChannels()[args]->setChannelOperators(this);

        std::string successMsg = "[COMMAND]: Channel " + args + " created.";
        sendMessage(successMsg, GREEN);
    }
}

// Methods

void Client::sendMessage(std::string const &msg, std::string const &color) {
    //std::cout << color << msg << RESET_COLOR << std::endl;
    (void)color;
    send(getClientFd(), msg.c_str(), msg.length(), 0);
}

void Client::sendFile(std::string const &file) {
    (void)file;
}

void Client::sendInvite(Client const *client) {
    (void)client;
}