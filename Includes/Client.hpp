/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:42:49 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/13 15:59:37 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

class Client {
    
    private:
    
    // Client's status
    
        bool    _isLogged;
        bool    _isBot;
        int     _pswdTries;
    
    // Client's connexion
    
        int         _clientFd;
        std::string _clientIp;

    // Client's informations

        std::string _hostname;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
    
    public:
    
    // Constructor
        
        Client(int newClientFd, std::string host);

    // Destructor

        virtual ~Client();

    // Getters
        
        int getClientFd() const;
        int getClientHostname() const;
        int getClientNickname() const;
        int getClientUsername() const;
        int getClientBuffer() const;
        int getClientBotStatus() const;
        int getClientLogStatus() const;
        int getClientPswdTries() const;
        
    // Setters
    
        void setClientFd(int clientFd);
        void setClientIp(std::string clientIp);
        void setClientHostname(std::string hostname) const;
        void setClientNickname(std::string nickname) const;
        void setClientUsername(std::string username) const;
        void setClientBuffer(std::string buffer) const;
        void setClientBotStatus(bool status) const;
        void setClientLogStatus(bool status) const;
        void setClientPswdTries(bool status) const;
};