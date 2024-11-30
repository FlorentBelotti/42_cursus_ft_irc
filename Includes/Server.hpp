/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:31:52 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/01 00:41:10 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Client.hpp"


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

    // Clients informations
    
        std::vector<Client>         _clients;
        std::vector<struct pollfd>  _clientsFd;
    
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
        bool    bindSocketToAdress();
        bool    setSocketOptions();

    // Epoll management

        bool    manageEpollAndEvents();

    // Client(s) management

        void    newClient();
        void    receiveDataFromClient();

    // Signal(s) management

        static void handleSignal(int sigId);

    // Server's cleaners

        void    closeFileDescriptors();
        void    clearClients(int fd);

    // Setters
        void    setServerFd(int serverFd);
        void    setServerStatus(bool status);
        void    setEpollFd(int epollFd);

    // Getters
        int                 getServerFd();
        int                 getEpollFd();
        int                 getServerPort();
        bool                getServerStatus();
        struct epoll_event  &getEpollEvent();

    // Animation
        static void    *serverIsRunningAnimation(void *);
};