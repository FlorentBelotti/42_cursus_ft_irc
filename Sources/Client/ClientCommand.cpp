/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 02:23:11 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/20 21:06:59 by fbelotti         ###   ########.fr       */
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

// Commands

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
        
        addClientChannel(args, server->getServerChannels()[args]);
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
        
        server->addServerChannel(args, new Channel(args));
        
        // Add channel to client
        
        addClientChannel(args, server->getServerChannels()[args]);

        // Add client to channel
        
        server->getServerChannels()[args]->addClient(this);

        // Add client to channel's operators

        server->getServerChannels()[args]->addChannelOperators(this);

        std::string successMsg = "[COMMAND]: Channel " + args + " created.";
        sendMessage(successMsg, GREEN);
    }
}

void Client::clientPartCommand(const std::string &args, Server *server) {
    
    // Check if channel exists
    
    if (server->getServerChannels().find(args) == server->getServerChannels().end()) {

        // Client join channel
        
        std::string successMsg = "[COMMAND]: Channel doesn't exist.";
        sendMessage(successMsg, GREEN);
        return;
    }
    
    else {
        
        // Remove client from channel
        
        server->getServerChannels()[args]->removeClient(this);
        
        // Remove channel from client
        
        removeClientChannel(args);

        // Remove client from channel's operators
        
        server->getServerChannels()[args]->removeOperator(this, server);

        std::string successMsg = "[COMMAND]: Channel " + args + " exited.";
        sendMessage(successMsg, GREEN);
    }
}

void    Client::clientPrivmsgCommand(const std::string &args, Server *server) {
    
    // Check command arguments
    
    std::vector<std::string> arguments = getArgsVector(args);
    
    if (arguments.size() < 2) {
        std::string errorMsg = "[USAGE]: PRIVMSG command requires a channel and a message.";
        sendMessage(errorMsg, RED);
        return;
    }

    std::string channelName = arguments[0];
    std::string message = args.substr(channelName.length() + 1); // Extraire le message

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "[USAGE]: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg, RED);
        return;
    }

    Channel* channel = it->second;

    // Send message
    
    std::string fullMessage = "[" + getClientNickname() + "]: " + message;
    channel->sendMessageToChannel(fullMessage, this);
}

void Client::clientQuitCommand(const std::string &args, Server *server) {
    
    sendMessage(args, RESET_COLOR);
    
    // close file_descriptor
    
    close(getClientFd());

    // Delete client from epoll
    
    epoll_ctl(server->getEpollFd(), EPOLL_CTL_DEL, getClientFd(), NULL);

    // Delete client from channel(s)
    
    std::map<std::string, Channel*> &channels = server->getServerChannels();
    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel *channel = it->second;
        channel->removeClient(this);
        channel->removeOperator(this, server);
    }

    // Delete client from server
    
    server->getClients().erase(getClientFd());
    delete this;

    std::string successMsg = "[COMMAND]: Client disconnected.";
    std::cout << successMsg << std::endl;
}

void Client::clientTopicCommand(const std::string &args, Server *server) {
    
    // Check command arguments
    
    std::vector<std::string> arguments = getArgsVector(args);

    if (arguments.size() < 2) {
        std::string errorMsg = "[USAGE]: /topic <channel> <topic>";
        sendMessage(errorMsg, RED);
        return;
    }

    std::string channelName = arguments[0];
    std::string topic = args.substr(channelName.length() + 1); // Extraire le topic

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "Error: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg, RED);
        return;
    }

    // Check if topic name is valid

    if (topic.length() > 255) {
        std::string errorMsg = "Error: Topic must be 255 characters max.";
        sendMessage(errorMsg, RED);
        return;
    }

    // Check if client is an operator
    
    Channel* channel = it->second;
    
    if (std::find(channel->getChannelOperators().begin(), channel->getChannelOperators().end(), this) == channel->getChannelOperators().end()) {
        std::string errorMsg = "Error: You are not an operator of channel " + channelName + ".";
        sendMessage(errorMsg, RED);
        return;
    }

    // Set topic
    
    channel->setChannelTopic(topic);
    std::string successMsg = "[COMMAND]: Topic set to " + topic;
    sendMessage(successMsg, GREEN);
}

void Client::clientInviteCommand(const std::string &args, Server *server) {
    
    std::vector<std::string> arguments = getArgsVector(args);

    // Check command arguments
    
    if (arguments.size() != 2) {
        std::string errorMsg = "[USAGE]: /invite <nickname> <channel>";
        sendMessage(errorMsg, RED);
        return;
    }

    std::string nickname = arguments[0];
    std::string channelName = arguments[1];

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "Error: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg, RED);
        return;
    }

    Channel* channel = it->second;

    // Check if client is an operator
    
    if (std::find(channel->getChannelOperators().begin(), channel->getChannelOperators().end(), this) == channel->getChannelOperators().end()) {
        std::string errorMsg = "Error: You are not an operator of channel " + channelName + ".";
        sendMessage(errorMsg, RED);
        return;
    }

    // Check if client exists

    Client* invitedClient = server->getClientByNickname(nickname);
    if (!invitedClient) {
        std::string errorMsg = "Error: Client " + nickname + " does not exist.";
        sendMessage(errorMsg, RED);
        return;
    }

    // Send invite
    
    std::string inviteMsg = "You have been invited to join channel " + channelName + " by " + getClientNickname() + ".";
    invitedClient->sendMessage(inviteMsg, GREEN);

    std::string successMsg = "Client " + nickname + " has been invited to channel " + channelName + ".";
    sendMessage(successMsg, GREEN);
}

void Client::clientKickCommand(const std::string &args, Server *server) {
    
    std::vector<std::string> arguments = getArgsVector(args);

    // Check command arguments
    
    if (arguments.size() < 2) {
        std::string errorMsg = "[USAGE]: /kick <nickname> <channel> <reason>";
        sendMessage(errorMsg, RED);
        return;
    }

    std::string nickname = arguments[0];
    std::string channelName = arguments[1];
    std::string reason = args.substr(nickname.length() + channelName.length() + 2);

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "Error: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg, RED);
        return;
    }

    Channel* channel = it->second;

    // Check if client is an operator
    
    if (std::find(channel->getChannelOperators().begin(), channel->getChannelOperators().end(), this) == channel->getChannelOperators().end()) {
        std::string errorMsg = "Error: You are not an operator of channel " + channelName + ".";
        sendMessage(errorMsg, RED);
        return;
    }

    // Check if client exists

    Client* kickedClient = server->getClientByNickname(nickname);
    if (!kickedClient) {
        std::string errorMsg = "Error: Client " + nickname + " does not exist.";
        sendMessage(errorMsg, RED);
        return;
    }

    // Kick client
    
    std::string kickMsg = "You have been kicked from channel " + channelName + " by " + getClientNickname() + " for " + reason + ".";
    kickedClient->sendMessage(kickMsg, RED);

    channel->removeClient(kickedClient);
    kickedClient->removeClientChannel(channelName);

    std::string successMsg = "Client " + nickname + " has been kicked from channel " + channelName + ".";
    sendMessage(successMsg, GREEN);
}
