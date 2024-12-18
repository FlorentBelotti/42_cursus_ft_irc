/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:31:52 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/17 13:46:28 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Client.hpp"
#include "Channel.hpp"
class Server {
    
    private:
    
    // Server informations
    
        std::string _serverPswd;
        sockaddr_in _serverAdress;
        bool        _serverStatus;
        int         _serverPort;
        int         _serverFd;
    
    // Epoll informations

        int                 _epollFd;
        struct epoll_event  _epollEvent;
        struct epoll_event  _epollEventsArr[CLIENT_NB];

    // Clients informations
    
        std::map<int, Client *>     _clients;
        std::vector<struct pollfd>  _clientsFd;
    
    // Channels informations
    
        std::map<std::string, Channel *> _channels;
    
    // Commands

        enum commands {
            NICKNAME, // Define user pseudo
            USER, // Define user infos : USER <username> <hostname> <servername> <realname>
            JOIN, // Join a channel : JOIN <#channel>
            PART, // Leave a channel : PART <#channel>
            PRIVMSG, // Send a message : PRIVMSG <#channel> :<message>
            NOTICE, // Send a notice : NOTICE <#channel> :<message>
            PING, // Ping the server : PING <server>
            PONG, // Pong the server : PONG <server>
            QUIT, // Quit the server : QUIT <message>
            TOPIC, // Set the topic : TOPIC <#channel> :<topic>
            INVITE, // Invite a user : INVITE <nickname> <#channel>
            KICK, // Kick a user : KICK <#channel> <nickname> :<reason>
        };
    
    // Error
    
        enum setupStep {
            CREATE_SOCKET,
            SET_NONBLOCKING,
            BIND_SOCKET,
            SET_OPTIONS,
            MANAGE_EPOLL
        };
        
    // Abstract
        
        Server();
    
    public:
    
    // Constructor

        // Set _serverFD = -1 | Security check for const usage (not your concern Romain)
        Server(std::string const &pswd, int const &port);
    
    // Destructor

        ~Server();
    
    // Socket(s) management

        void    setupSocketAndEvents();
        bool    createSocket();
        bool    setSocketNonBlockingMode();
        bool    bindSocketToAddress();
        bool    setSocketOptions();

    // Epoll management

        bool    manageEpollAndEvents();

    // Loop

        void    serverLoop();
        void    handleClientEvent(int user_fd);
        void    handleNewClient();
        void    handleMessage(std::string const &msg, int user_fd);

    // Server's cleaners

        void    closeFileDescriptors();
        void    clearClients();

    // Error(s) management
    
        void    manageSocketError(int step);
    
    // Setters
        void    setServerFd(int serverFd);
        void    setServerStatus(bool status);
        void    setEpollFd(int epollFd);

    // Getters
        int                     getServerFd() const;
        int                     getEpollFd() const ;
        int                     getServerPort() const ;
        bool                    getServerStatus() const ;
        struct epoll_event      &getEpollEvent() ;
        struct epoll_event      *getEpollEventsArr() ;
        std::map<int, Client *> &getClients() ;

    // Animation
        static void    *serverIsRunningAnimation(void *);

    // Dashboard
        static void *startDashboard(void *arg);
        void        printDashboard() const;
};