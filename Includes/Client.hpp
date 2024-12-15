/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:42:49 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/15 19:34:20 by fbelotti         ###   ########.fr       */
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
        
        Client(int newClientFd, std::string hostname);

    // Destructor

        virtual ~Client();

    // Getters
        
        std::string getClientHostname() const;
        std::string getClientNickname() const;
        std::string getClientUsername() const;
        std::string getClientBuffer() const;
        bool        getClientBotStatus() const;
        bool        getClientLogStatus() const;
        int         getClientFd() const;
        int         getClientPswdTries() const;
        
    // Setters
    
        void setClientFd(int clientFd);
        void setClientIp(std::string clientIp);
        void setClientHostname(std::string hostname);
        void setClientNickname(std::string nickname);
        void setClientUsername(std::string username);
        void setClientBuffer(std::string buffer);
        void setClientBotStatus(bool status);
        void setClientLogStatus(bool status);
        void setClientPswdTries(int tries);

    // Methods

        
};