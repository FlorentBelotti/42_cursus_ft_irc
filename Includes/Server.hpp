/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:31:52 by fbelotti          #+#    #+#             */
/*   Updated: 2024/11/29 15:54:34 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Client.hpp"

class Server {
    
    private:
    
    // Server informations
    
        std::string _serverPswd;
        static bool _serverStatus;
        int         _serverPort;
        int         _serverFd;

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

        void    createSocket();

    // Client(s) management

        void    newClient();
        void    receiveDataFromClient();

    // Signal(s) management

        static void handleSignal(int sigId);

    // Server's cleaners

        void    closeFileDescriptors();
        void    clearClients(int fd);
};