/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 02:23:11 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/26 23:20:32 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Client.hpp"

// Debug

void printClientNicknames(const std::vector<Client*>& clients) {
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        std::cout << (*it)->getClientNickname() << std::endl;
    }
}

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
    int i = 0;
    while (iss >> token) {
        std::cout << MAGENTA << "arguments[" << i++ << "] : " << token << std::endl;
        argsVector.push_back(token);
    }
    return argsVector;
}

// Commands

void Client::clientPassCommand(const std::string &args, Server *server) {
    
    // Check args
    
    if (_isLogged) {
        std::string errorMsg = "Error: You are already logged in.";
        sendMessage(errorMsg);
        return;
    }

    if (args.empty() || args.find(' ') != std::string::npos) {
        std::string errorMsg = "[USAGE]: /pass <password>";
        sendMessage(errorMsg);
        return;
    }
    
    if (getClientPswdTries() > 3) {
        std::string errorMsg = "Error: Too many password tries.";
        sendMessage(errorMsg);
        close(getClientFd());
        epoll_ctl(server->getEpollFd(), EPOLL_CTL_DEL, getClientFd(), NULL);
        server->getClients().erase(getClientFd());  
        delete this;
        return;
    }

    std::string correctPassword = server->getServerPswd();
    if (args != correctPassword) {
        std::string errorMsg = "Error: Incorrect password.";
        sendMessage(errorMsg);
        addClientPswdTry();
        _isLogged = false;
        return;
    } else {
        setClientPassword(args);
        _isLogged = true;
        std::string successMsg = "You are now successefully logged in.";
        sendMessage(successMsg);
        std::cout << GREEN << "[COMMAND]: Password set." << RESET_COLOR << std::endl;   
    }
}

void Client::clientNicknameCommand(const std::string &args) {
    
    // if (!_isLogged) {
    //     std::string errorMsg = "Error: You are not logged in. Please set your password first.";
    //     sendMessage(errorMsg);
    //     return;
    // }

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
    setClientPswdTries(0);
    std::string successMsg = "[COMMAND]: Client infos updated\n";
    // successMsg += "Username: " + getClientUsername() + "\n";
    // successMsg += "Hostname: " + getClientHostname() + "\n";
    // successMsg += "Servername: " + getClientServername() + "\n";
    // successMsg += "Realname: " + getClientRealname() + "\n";
    std::cout << GREEN << successMsg << RESET_COLOR << std::endl;
}

void Client::clientJoinCommand(const std::string &args, Server *server) {
    
    if (!_isLogged) {
        std::string errorMsg = "Error: You are not logged in. Please set your password first.";
        sendMessage(errorMsg);
        return;
    }

    // Join channel if it exists
    
    if (server->getServerChannels().find(args) != server->getServerChannels().end()) {

        Channel* channel = server->getServerChannels()[args];
        if (channel->getChannelStatus()) {
            if (channel->isInvited(this)) {
                std::cout << getClientUsername() << " Join a restricted channel" << std::endl;
                addClientChannel(args, channel);
                channel->addClient(this);
                channel->broadcast(":" + getClientNickname() + " JOIN " + channel->getChannelName() + "\r\n");
            }
            else {
                std::string errorMsg = "Error: You are not invited to " + channel->getChannelName() + ".";
                sendMessage(errorMsg);
                return;
            }
        } else {
            std::cout << getClientUsername() << " Join an already existing channel" << std::endl;
            addClientChannel(args, channel);
            channel->addClient(this);
            channel->broadcast(":" + getClientNickname() + " JOIN " + channel->getChannelName() + "\r\n");   
        }
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
        std::cout << getClientUsername() << " Join a new channel" << std::endl;
        server->addServerChannel(args, new Channel(args));
        Channel* channel = server->getServerChannels()[args];
        addClientChannel(args, channel);
        channel->addClient(this);
        channel->addChannelOperators(this);
        channel->broadcast(":" + getClientNickname() + " JOIN " + channel->getChannelName() + "\r\n");
    }
}

void Client::clientPartCommand(const std::string &args, Server *server) {

    if (!_isLogged) {
        std::string errorMsg = "Error: You are not logged in. Please set your password first.";
        sendMessage(errorMsg);
        return;
    }
    
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
    
    Channel *channel = server->getServerChannels()[channelName];
    channel->removeClient(this);
    removeClientChannel(channelName);
    channel->removeOperator(this);
    channel->broadcast(":" + getClientNickname() + " PART " + channel->getChannelName() + "\r\n");
    sendMessage(":" + getClientNickname() + " PART " + channel->getChannelName() + "\r\n");
}

void    Client::clientPrivmsgCommand(const std::string &args, Server *server) {
    

    if (!_isLogged) {
        std::string errorMsg = "Error: You are not logged in. Please set your password first.";
        sendMessage(errorMsg);
        return;
    }
    
    // Check command arguments
    
    std::vector<std::string> arguments = getArgsVector(args);
    
    if (arguments.size() < 2) {
        std::string errorMsg = "[USAGE]: PRIVMSG command requires a channel and a message.";
        sendMessage(errorMsg);
        return;
    }

    std::string target = arguments[0];
    std::string message = args.substr(target.length() + 1); // Extraire le message
    if (!message.empty() && message[0] == ':') {
        message = message.substr(1);
    }

    if (target[0] == '#') {
        Channel *channel = server->getServerChannels().find(target)->second;
        if (!channel)
            return;
        std::vector<Client*> clients = channel->getChannelClients();
        if (std::find(clients.begin(), clients.end(), this) != clients.end()) {
            channel->restrictedBroadcast(":" + getClientNickname() + " PRIVMSG " + channel->getChannelName() + " :" + message + "\r\n", this);
        } else {
            std::string errorMsg = "[USAGE]: You are not registered on " + target + ".";
            sendMessage(errorMsg);
            return ;
        }
    }
    else {
        Client *targetClient = server->getClientByNickname(target);
        if (!targetClient) {
            std::string errorMsg = "[USAGE]: Client " + target + " does not exist.";
            sendMessage(errorMsg);
            return;
        }
        targetClient->sendMessage(":" + getClientNickname() + " PRIVMSG " + target + " :" + message + "\r\n");
    }
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
    
    if (!_isLogged) {
        std::string errorMsg = "Error: You are not logged in. Please set your password first.";
        sendMessage(errorMsg);
        return;
    }
    
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
    
    if (!channel->isOperator(this)) {
        std::string errorMsg = "Error: You are not an operator of channel " + channelName + ".";
        sendMessage(errorMsg);
        return;
    }

    // Set topic
    
    channel->setChannelTopic(topic);
    std::string topicMsg = " :" + topic;
    channel->broadcast(":" + getClientNickname() + " TOPIC " + channel->getChannelName() + " :" + topic + "\r\n");
    std::cout << GREEN << "[COMMAND]: " << channel->getChannelName() << " topic set to " + topic << RESET_COLOR << std::endl;
}

void Client::clientInviteCommand(const std::string &args, Server *server) {

    if (!_isLogged) {
        std::string errorMsg = "Error: You are not logged in. Please set your password first.";
        sendMessage(errorMsg);
        return;
    }
    
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
    
    if (!channel->isOperator(this)) {
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
    
    std::cout << "invitation to " << invitedClient->getClientNickname() << std::endl;
    channel->addChannelInvitedClient(invitedClient);
    std::string inviteMsg = ":" + getClientNickname() + " INVITE " + invitedClient->getClientNickname() + " " + channelName + "\r\n";
    invitedClient->sendMessage(inviteMsg);
    sendMessage(inviteMsg);
}

void Client::clientKickCommand(const std::string &args, Server *server) {

    if (!_isLogged) {
        std::string errorMsg = "Error: You are not logged in. Please set your password first.";
        sendMessage(errorMsg);
        return;
    }
    
    std::vector<std::string> arguments = getArgsVector(args);

    // Check command arguments
    
    if (arguments.size() < 3) {
        std::string errorMsg = "[USAGE]: /kick <nickname> <channel> <reason>";
        sendMessage(errorMsg);
        return;
    }


    std::string channelName = arguments[0];
    std::string nickname = arguments[1];
    std::cout << RESET_COLOR << "nick: " << nickname << std::endl;
    std::cout << RESET_COLOR << "chan: " << channelName << std::endl;
    std::string reason;
    for (std::vector<std::string>::size_type i = 2; i < arguments.size(); ++i) {
        if (i > 2)
            reason += " ";
        reason += arguments[i];
    }

    // Check if channel exists

    std::map<std::string, Channel*>::iterator it = server->getServerChannels().find(channelName);
    if (it == server->getServerChannels().end()) {
        std::string errorMsg = "[USAGE]: Channel " + channelName + " does not exist.";
        sendMessage(errorMsg);
        return;
    }


    Channel* channel = it->second;
    
    // Check if client is an operator
    
    if (!channel->isOperator(this)) {
        std::string errorMsg = "Error: You are not an operator of channel " + channelName + ".";
        sendMessage(errorMsg);
        return;
    }


    // Check if client exists

    Client* kickedClient = server->getClientByNickname(nickname);
    if (!kickedClient) {
        std::string errorMsg = "[USAGE]: Client " + nickname + " does not exist.";
        sendMessage(errorMsg);
        return;
    }


    if (kickedClient == this) {
        std::string errorMsg = "[USAGE]: You cannot kick yourself.";
        sendMessage(errorMsg);
        return;
    }

    // Kick client
    
    std::string kickMsg = ":" + getClientNickname() + " KICK " + channelName + " " + nickname + " :" + reason + "\r\n";
    channel->removeClient(kickedClient);
    kickedClient->removeClientChannel(channelName);
    channel->broadcast(kickMsg);
    kickedClient->sendMessage(kickMsg);
}

void Client::clientModeCommand(const std::string &args, Server *server) {
    
    std::vector<std::string> arguments = getArgsVector(args);
    
    Channel *channel = server->getServerChannels().find(arguments[0])->second;
    if (!channel) {
        std::string errorMsg = "Error: Channel " + arguments[0] + " does not exist.";
        sendMessage(errorMsg);
        return;
    }

    if (!channel->isOperator(this)) {
        std::string errorMsg = "Error: You are not an operator of channel " + channel->getChannelName() + ".";
        sendMessage(errorMsg);
        return;
    }

    if (arguments[1] == "+i") {
        channel->setChannelStatus(true);
        sendMessage(":" + getClientNickname() + " MODE " + channel->getChannelName() + " +i\r\n");
    } else if (arguments[1] == "-i") {
        channel->setChannelStatus(false);
        sendMessage(":" + getClientNickname() + " MODE " + channel->getChannelName() + " -i\r\n");
    }
}
