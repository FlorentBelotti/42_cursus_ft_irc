/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:21:28 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/17 13:24:23 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Channel.hpp"

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

// Setters

void Channel::setChannelName(std::string channelName) {
    _channelName = channelName;
}

void Channel::setChannelTopic(std::string channelTopic) {
    _channelTopic = channelTopic;
}

void Channel::setChannelPassword(std::string channelPassword) {
    _channelPassword = channelPassword;
}

// Methods

void Channel::addClient(Client *client) {
    _channelClients.push_back(client);
}

void Channel::removeClient(Client *client) {
    for (std::vector<Client*>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++) {
        if (*it == client) {
            _channelClients.erase(it);
            return;
        }
    }
}

void Channel::sendMessageToChannel(std::string message) {
    for (std::vector<Client*>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++) {
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