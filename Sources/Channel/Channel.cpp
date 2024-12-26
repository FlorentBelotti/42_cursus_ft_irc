/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:21:28 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/26 23:18:40 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Channel.hpp"

// Constructor

Channel::Channel(std::string channelName) : _channelName(channelName) {
    _channelTopic = "";
    _channelPassword = "";
}

// Destructor

Channel::~Channel() {
    _channelClients.clear();
}

// Getters

std::string Channel::getChannelName() const {
    return _channelName;
}

std::string Channel::getChannelTopic() const {
    return _channelTopic;
}

std::string Channel::getChannelPassword() const {
    return _channelPassword;
}

std::vector<Client*> Channel::getChannelClients() const {
    return _channelClients;
}

std::vector<Client*> Channel::getChannelOperators() const {
    return _channelOperators;
}

bool Channel::getChannelStatus() const {
    return _isRestricted;
}

// Setters

void Channel::setChannelStatus(bool status) {
    _isRestricted = status;
}

void Channel::setChannelName(std::string channelName) {
    _channelName = channelName;
}

void Channel::setChannelTopic(std::string channelTopic) {
    _channelTopic = channelTopic;
}

void Channel::setChannelPassword(std::string channelPassword) {
    _channelPassword = channelPassword;
}

void Channel::addChannelOperators(Client *client) {
    _channelOperators.push_back(client);
}

void Channel::addChannelInvitedClient(Client *client) {
    _channelInvited.push_back(client);
}

void Channel::removeOperator(Client *client) {
    std::vector<Client*>::iterator it = std::find(_channelOperators.begin(), _channelOperators.end(), client);
    if (it != _channelOperators.end()) {
        _channelOperators.erase(it);
    }
    // if (_channelOperators.empty()) {
    //     server->removeServerChannel(getChannelName());
    // }
}

// Methods

void Channel::addClient(Client *client) {
    _channelClients.push_back(client);
}

void Channel::removeClient(Client *client) {
    _channelClients.erase(std::remove(_channelClients.begin(), _channelClients.end(), client), _channelClients.end());
}

void Channel::restrictedBroadcast(std::string message, Client *sender) {
    for (std::vector<Client*>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++) {
        if (*it != sender) {
            std::cout << YELLOW << "[SERVER]:" << RESET_COLOR << " Sending message to " << (*it)->getClientNickname() << std::endl;
            (*it)->sendMessage(message);
        }
    }
}

void Channel::broadcast(std::string message) {
    for (std::vector<Client*>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++) {
            std::cout << YELLOW << "[SERVER]:" << RESET_COLOR << " Sending message to " << (*it)->getClientNickname() << std::endl;
            (*it)->sendMessage(message);
    }
}

void Channel::sendFileToChannel(std::string message, Client *client) {
    for (std::vector<Client*>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++) {
        if (*it != client) {
            (*it)->sendFile(message);
        }
    }
}

void Channel::sendInviteToChannel(Client *client) {
    for (std::vector<Client*>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++) {
        (*it)->sendInvite(client);
    }
}

bool Channel::isOperator(const Client* client) const {
    return std::find(_channelOperators.begin(), _channelOperators.end(), client) != _channelOperators.end();
}

bool Channel::isInvited(const Client* client) const {
    return std::find(_channelInvited.begin(), _channelInvited.end(), client) != _channelInvited.end();
}