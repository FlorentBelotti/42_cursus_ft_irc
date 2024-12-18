/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 02:23:11 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/20 02:41:48 by fbelotti         ###   ########.fr       */
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
        std::string errorMsg = "Error: PRIVMSG command requires a channel and a message.";
        sendMessage(errorMsg, RED);
        return;
    }

    std::string channelName = arguments[0];
    std::string message = args.substr(channelName.length() + 1); // Extraire le message

    // Check if channel exists
    
    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "Error: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg, RED);
        return;
    }

    Channel* channel = it->second;

    // Send message
    
    std::string fullMessage = "[" + getClientNickname() + "]: " + message;
    channel->sendMessageToChannel(fullMessage, this);
}
