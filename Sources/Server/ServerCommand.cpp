/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:45:41 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/20 21:05:23 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"

void Server::handleMessage(std::string const &msg, int user_fd) {

    // Check if the message is a command

    if (!msg.empty() && msg[0] == '/') {
        
        // Extract command and its arguments
        
        size_t  spacePos = msg.find(' ');
        std::string command = msg.substr(1, spacePos - 1);
        std::string args = (spacePos != std::string::npos) ? msg.substr(spacePos + 1) : "";
        
        // Process command

        processClientCommand(command, args, user_fd);

    } else {
         
        // Handle server's response
        
        std::cout << MAGENTA << "[" << getClients()[user_fd]->getClientHostname() << "]: " << RESET_COLOR << msg << std::endl;
        send(user_fd, "Message received", 16, 0);
        
        // Handle client's buffer
        
        getClients()[user_fd]->setClientBuffer("");
    }
}

Server::commands Server::defineCommand(const std::string &command) {
    if (command == "nickname") return NICKNAME;
    if (command == "user") return USER;
    if (command == "join") return JOIN;
    if (command == "part") return PART;
    if (command == "privmsg") return PRIVMSG;
    if (command == "quit") return QUIT;
    if (command == "topic") return TOPIC;
    if (command == "invite") return INVITE;
    if (command == "kick") return KICK;
    if (command == "help") return HELP;
    throw std::invalid_argument("Unknown command, try </help>");
}

void Server::processClientCommand(const std::string& command, const std::string& args, int user_fd) {
    try {
        commands cmd = defineCommand(command);
        switch (cmd) {
            case NICKNAME:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /NICKNAME command with args: " << args << std::endl;
                getClients()[user_fd]->clientNicknameCommand(args);
                break;
            case USER:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /USER command with args: " << args << RESET_COLOR << std::endl;
                getClients()[user_fd]->clientUserCommand(args);
                break;
            case JOIN:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /JOIN command with args: " << args << RESET_COLOR << std::endl;
                getClients()[user_fd]->clientJoinCommand(args, this);
                break;
            case PART:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /PART command with args: " << args << RESET_COLOR << std::endl;
                getClients()[user_fd]->clientPartCommand(args, this);
                break;
            case PRIVMSG:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /PRIVMSG command with args: " << args << RESET_COLOR << std::endl;
                getClients()[user_fd]->clientPrivmsgCommand(args, this);
                break;
            case QUIT:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /QUIT command with args: " << args << RESET_COLOR << std::endl;
                getClients()[user_fd]->clientQuitCommand(args, this);
                break;
            case TOPIC:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /TOPIC command with args: " << args << RESET_COLOR << std::endl;
                getClients()[user_fd]->clientTopicCommand(args, this);
                break;
            case INVITE:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /INVITE command with args: " << args << RESET_COLOR << std::endl;
                getClients()[user_fd]->clientInviteCommand(args, this);
                break;
            case KICK:
                std::cout << YELLOW << "[SERVER]: " << RESET_COLOR << "Processing /KICK command with args: " << args << RESET_COLOR << std::endl;
                getClients()[user_fd]->clientKickCommand(args, this);
                break;
            default:
                std::string errorMsg = "Unknown command: " + command;
                send(user_fd, errorMsg.c_str(), errorMsg.length(), 0);
                break;
        }
    } catch (const std::invalid_argument& e) {
        std::string errorMsg = "Unknown command: " + command;
        send(user_fd, errorMsg.c_str(), errorMsg.length(), 0);
    }
}