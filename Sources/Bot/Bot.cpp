/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 14:18:53 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/31 16:00:53 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Bot.hpp"
#include <iostream>

Bot::Bot(int newClientFd, std::string hostname) : Client(newClientFd, hostname) {
    setClientNickname("Bot");
    setClientUsername("Bot");
    setClientRealname("Bot");
    setClientLogStatus(true);
    setClientBotStatus(true);
}

void Bot::handleBotCommand(const std::string &message, Channel *channel) {
    if (message == "!hello") {
        std::cout << YELLOW << "[SERVER]:" << RESET_COLOR << " Bot response" << std::endl;
        channel->restrictedBroadcast(":" + this->getClientNickname() + " PRIVMSG " + channel->getChannelName() + " :" + "Hello!" + "\r\n", this);
    }
}