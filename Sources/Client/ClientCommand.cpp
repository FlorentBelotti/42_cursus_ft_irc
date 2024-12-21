/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 02:23:11 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/21 19:46:03 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Client.hpp"

// Commands utils

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

void Client::sendRequest(const std::string &nickname, const std::string &cmd, Channel *channel, const std::string &other) {
    std::string request = ":" + nickname + " " + cmd + " " + channel->getChannelName() + other + "\r\n";
    channel->sendMessageToChannel(request, this);
}

// Commands

void Client::clientPassCommand(const std::string &args) {
    
    // Check args
    
    if (args.empty() || args.find(' ') != std::string::npos) {
        std::string errorMsg = "[USAGE]: /pass <password>";
        sendMessage(errorMsg);
        return;
    }
    
    setClientPassword(args);
    std::cout << GREEN << "[COMMAND]: Password set." << RESET_COLOR << std::endl;
}

void Client::clientNicknameCommand(const std::string &args) {
    
    if (args.empty() || args.find(' ') != std::string::npos) {
        std::string errorMsg = "[USAGE]: /nickname <nickname>";
        std::cout << "not valid" << std::endl;
        sendMessage(errorMsg);
        return;
    }

    if (!isValidName(args, 9)) {
        std::string errorMsg = "[USAGE]: Nickname must be between 1 and 9 characters and contain only letters, digits, '_', and '-'.";
        sendMessage(errorMsg);
        std::cout << "not valid" << std::endl;
        return;
    }

    setClientNickname(args);
    std::cout << GREEN << "[COMMAND]: Nickname changed to " + getClientNickname() << RESET_COLOR << std::endl;
}

void Client::clientUserCommand(const std::string &args) {
    
    std::vector<std::string> argsVector = getArgsVector(args);
    
    if (argsVector.size() != 4) {
        std::string errorMsg = "[USAGE]: /username <username> <hostname> <servername> <realname>";
        sendMessage(errorMsg);
        return;
    }

    setClientUsername(argsVector[0]);
    setClientHostname(argsVector[1]);
    setClientServername(argsVector[2]);
    setClientRealname(argsVector[3]);
    std::string successMsg = "[COMMAND]: Client infos updated\n";
    // successMsg += "Username: " + getClientUsername() + "\n";
    // successMsg += "Hostname: " + getClientHostname() + "\n";
    // successMsg += "Servername: " + getClientServername() + "\n";
    // successMsg += "Realname: " + getClientRealname() + "\n";
    std::cout << GREEN << successMsg << RESET_COLOR << std::endl;
}

void Client::clientJoinCommand(const std::string &args, Server *server) {
    
    // Join channel if it exists
    
    if (server->getServerChannels().find(args) != server->getServerChannels().end()) {

        // Client join channel
        
        addClientChannel(args, server->getServerChannels()[args]);
        server->getServerChannels()[args]->addClient(this);
        sendRequest(getClientNickname(), "JOIN", server->getServerChannels()[args], "");
    } 
    
    // Create Channel if it doesn't exist
    
    else {
        
        // Check if channel name is valid
        
        if (args[0] != '#' && args[0] != '&') {
            std::string errorMsg = "[USAGE]: Channel name must start with a '#' or '&' character.";
            sendMessage(errorMsg);
            return;
        }
        if (!isValidName(&args[1], 9)) {
            std::string errorMsg = "[USAGE]: Channel name must be between 1 and 9 characters and contain only letters, digits, '_', and '-'.";
            sendMessage(errorMsg);
            return;
        }

        server->addServerChannel(args, new Channel(args));
        Channel* channel = server->getServerChannels()[args];
        addClientChannel(args, channel);
        channel->addClient(this);
        channel->addChannelOperators(this);
        sendRequest(getClientNickname(), "JOIN", channel, "");
    }
}

void Client::clientPartCommand(const std::string &args, Server *server) {

    // Check if channel exists
    
    size_t spacePos = args.find(' ');
    if (spacePos == std::string::npos) {
        std::string errorMsg = "[USAGE]: /part <#channel>";
        sendMessage(errorMsg);
        return;
    }
    std::string channelName = args.substr(0, spacePos);
    if (channelName[0] != '#' && channelName[0] != '&') {
        std::string errorMsg = "[USAGE]: Channel name must start with a '#' or '&' character.";
        sendMessage(errorMsg);
        return;
    }
    if (server->getServerChannels().find(channelName) == server->getServerChannels().end()) {
        std::string errorMsg = "[USAGE]: Channel doesn't exist.";
        sendMessage(errorMsg);
        return;
    }
    
    sendRequest(getClientNickname(), "PART", server->getServerChannels()[channelName], "");
    server->getServerChannels()[channelName]->removeClient(this);
    removeClientChannel(channelName);
    server->getServerChannels()[channelName]->removeOperator(this);
}

void    Client::clientPrivmsgCommand(const std::string &args, Server *server) {
    
    // Check command arguments
    
    std::vector<std::string> arguments = getArgsVector(args);
    
    if (arguments.size() < 2) {
        std::string errorMsg = "[USAGE]: PRIVMSG command requires a channel and a message.";
        sendMessage(errorMsg);
        return;
    }

    std::string channelName = arguments[0];
    std::string message = args.substr(channelName.length() + 1); // Extraire le message

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "[USAGE]: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg);
        return;
    }

    Channel* channel = it->second;

    // Send message
    
    std::string fullMessage = "[" + getClientNickname() + "]: " + message;
    channel->sendMessageToChannel(fullMessage, this);
}

void Client::clientQuitCommand(const std::string &args, Server *server) {
    
    std::cout << YELLOW << "[SERVER]:" << RESET_COLOR << " Client " << getClientNickname() << " has been disconnected." << std::endl;
    
    sendMessage(args);
    
    // close file_descriptor
    
    close(getClientFd());

    // Delete client from epoll    

    epoll_ctl(server->getEpollFd(), EPOLL_CTL_DEL, getClientFd(), NULL);

    // Delete client from channel(s)
    
    std::map<std::string, Channel*> &channels = server->getServerChannels();
    
    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
        Channel *channel = it->second;
        channel->removeClient(this);
        channel->removeOperator(this);
    }

    // Delete client from server

    server->getClients().erase(getClientFd());
        
    delete this;

}

void Client::clientTopicCommand(const std::string &args, Server *server) {
    
    // Check command arguments
    
    std::vector<std::string> arguments = getArgsVector(args);

    if (arguments.size() < 2) {
        std::string errorMsg = "[USAGE]: /topic <channel> <topic>";
        sendMessage(errorMsg);
        return;
    }

    std::string channelName = arguments[0];
    std::string topic = args.substr(channelName.length() + 1); // Extraire le topic

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "Error: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg);
        return;
    }

    // Check if topic name is valid

    if (topic.length() > 255) {
        std::string errorMsg = "Error: Topic must be 255 characters max.";
        sendMessage(errorMsg);
        return;
    }

    // Check if client is an operator
    
    Channel* channel = it->second;
    
    if (std::find(channel->getChannelOperators().begin(), channel->getChannelOperators().end(), this) == channel->getChannelOperators().end()) {
        std::string errorMsg = "Error: You are not an operator of channel " + channelName + ".";
        sendMessage(errorMsg);
        return;
    }

    // Set topic
    
    channel->setChannelTopic(topic);
    std::string topicMsg = " :" + topic;
    sendRequest(getClientNickname(), "TOPIC", channel, topicMsg);
    std::cout << GREEN << "[COMMAND]: " << channel->getChannelName() << " topic set to " + topic << RESET_COLOR << std::endl;
}

void Client::clientInviteCommand(const std::string &args, Server *server) {
    
    std::vector<std::string> arguments = getArgsVector(args);

    // Check command arguments
    
    if (arguments.size() != 2) {
        std::string errorMsg = "[USAGE]: /invite <nickname> <channel>";
        sendMessage(errorMsg);
        return;
    }

    std::string nickname = arguments[0];
    std::string channelName = arguments[1];

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "Error: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg);
        return;
    }

    Channel* channel = it->second;

    // Check if client is an operator
    
    if (std::find(channel->getChannelOperators().begin(), channel->getChannelOperators().end(), this) == channel->getChannelOperators().end()) {
        std::string errorMsg = "Error: You are not an operator of channel " + channelName + ".";
        sendMessage(errorMsg);
        return;
    }

    // Check if client exists

    Client* invitedClient = server->getClientByNickname(nickname);
    if (!invitedClient) {
        std::string errorMsg = "Error: Client " + nickname + " does not exist.";
        sendMessage(errorMsg);
        return;
    }

    // Send invite
    
    std::string inviteMsg = "You have been invited to join channel " + channelName + " by " + getClientNickname() + ".";
    invitedClient->sendMessage(inviteMsg);

    std::string successMsg = "Client " + nickname + " has been invited to channel " + channelName + ".";
    sendMessage(successMsg);
}

void Client::clientKickCommand(const std::string &args, Server *server) {
    
    std::vector<std::string> arguments = getArgsVector(args);

    // Check command arguments
    
    if (arguments.size() < 2) {
        std::string errorMsg = "[USAGE]: /kick <nickname> <channel> <reason>";
        sendMessage(errorMsg);
        return;
    }

    std::string nickname = arguments[0];
    std::string channelName = arguments[1];
    std::string reason = args.substr(nickname.length() + channelName.length() + 2);

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "Error: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg);
        return;
    }

    Channel* channel = it->second;

    // Check if client is an operator
    
    if (std::find(channel->getChannelOperators().begin(), channel->getChannelOperators().end(), this) == channel->getChannelOperators().end()) {
        std::string errorMsg = "Error: You are not an operator of channel " + channelName + ".";
        sendMessage(errorMsg);
        return;
    }

    // Check if client exists

    Client* kickedClient = server->getClientByNickname(nickname);
    if (!kickedClient) {
        std::string errorMsg = "Error: Client " + nickname + " does not exist.";
        sendMessage(errorMsg);
        return;
    }

    // Kick client
    
    std::string kickMsg = "You have been kicked from channel " + channelName + " by " + getClientNickname() + " for " + reason + ".";
    kickedClient->sendMessage(kickMsg);

    channel->removeClient(kickedClient);
    kickedClient->removeClientChannel(channelName);

    std::string successMsg = "Client " + nickname + " has been kicked from channel " + channelName + ".";
    sendMessage(successMsg);
}
