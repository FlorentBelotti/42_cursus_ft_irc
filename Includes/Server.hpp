/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:31:52 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/12 18:12:41 by fbelotti         ###   ########.fr       */
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
        struct epoll_event  _epollEventsArr[CLIENT_NB];

    // Clients informations
    
        std::vector<Client>         _clients;
        std::vector<struct pollfd>  _clientsFd;
    
    // Error

    enum SetupStep {
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

    // Client(s) management

        void    newClient();
        void    receiveDataFromClient();

    // Server's cleaners

        void    closeFileDescriptors();
        void    clearClients(int fd);

    // Error(s) management
    
        void    manageSocketError(int step);
    
    // Setters
        void    setServerFd(int serverFd);
        void    setServerStatus(bool status);
        void    setEpollFd(int epollFd);

    // Getters
        int                 getServerFd() const;
        int                 getEpollFd() const ;
        int                 getServerPort() const ;
        bool                getServerStatus() const ;
        struct epoll_event  &getEpollEvent() ;
        struct epoll_event  *getEpollEventsArr() ;

    // Animation
        static void    *serverIsRunningAnimation(void *);

    // Dashboard
        static void *startDashboard(void *arg);
        void        printDashboard() const;
};