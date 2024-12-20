/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:29:02 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/20 02:19:31 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "utils.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Client;

class Server;
class Channel {
    
    private:
    
    // Channel's informations
    
        std::string _channelName;
        std::string _channelTopic;
        std::string _channelPassword;
        std::vector<Client*> _channelClients;
        std::vector<Client*> _channelOperators;
    
    public:
    
    // Constructor
        
        Channel(std::string channelName);
    
    // Destructor
    
        virtual ~Channel();
    
    // Getters
        
        std::string getChannelName() const;
        std::string getChannelTopic() const;
        std::string getChannelPassword() const;
        std::vector<Client*> getChannelClients() const;
        std::vector<Client*> getChannelOperators() const;
        
    // Setters
    
        void setChannelName(std::string channelName);
        void setChannelTopic(std::string channelTopic);
        void setChannelPassword(std::string channelPassword);
        void addChannelOperators(Client *client);
        void removeOperator(Client *client, Server *server);
        
    // Methods
    
        void addClient(Client *client);
        void removeClient(Client *client);
        void sendMessageToChannel(std::string message, Client *sender);
        void sendFileToChannel(std::string message, Client *sender);
        void sendInviteToChannel(Client *client);
};