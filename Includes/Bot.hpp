/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 14:18:17 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/31 15:52:46 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Client.hpp"
#include "Server.hpp"

class Bot : public Client {
public:
    Bot(int newClientFd, std::string hostname);
    void handleBotCommand(const std::string &message, Channel *channel);
};