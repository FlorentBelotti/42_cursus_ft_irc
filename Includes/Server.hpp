/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:31:52 by fbelotti          #+#    #+#             */
/*   Updated: 2024/11/22 18:03:36 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Client.hpp"

class Server {
    
    private:
    
    // Server informations
    std::string _serverPswd;
    bool        _serverStatus;
    int         _serverPort;
    int         _serverFd;

    // Clients informations
    std::vector<Client>         _clients;
    std::vector<struct pollfd>  _clientsFd;
    
    public:
    
    // Constructor
    Server();
    Server(std::string pswd, int port);
    
}