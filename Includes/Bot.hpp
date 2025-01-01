/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 14:18:17 by fbelotti          #+#    #+#             */
/*   Updated: 2025/01/01 17:25:56 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Client.hpp"
#include "Server.hpp"

class Bot : public Client {

    public:

    // Constructor
    
    Bot(int newClientFd, std::string hostname);
    
    // Commands

    enum botCommands {
        HELLO,
        LEAVE,
        HELP
    };

    void                botHelloCommand(Channel *channel, Client *target);
    void                botHelpCommand(Channel *channel);
    void                handleBotCommand(const std::string &message, Channel *channel, Client *target, Server *server);
    Bot::botCommands    botDefineCommand(const std::string &command);
};