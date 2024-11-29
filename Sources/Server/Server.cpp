/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:25:19 by fbelotti          #+#    #+#             */
/*   Updated: 2024/11/29 21:04:24 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

Server::Server(std::string const &pswd, int const &port) : _serverPswd(pswd), _serverPort(port) {
    
    std::cout << YELLOW << "Server: Waiting for connexion";
    for (int i = 0; i != 3 ; i++)
        std::cout << ".";
    std::cout << std::endl;
}