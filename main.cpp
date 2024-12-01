/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:35:59 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/01 18:56:37 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Server.hpp"

int main(int ac, char **av) {
    
    //Usage check.
    if (ac != 3) {
        std::cerr << RED << "Usage: ./ircserv <port> <password>" << RESET_COLOR << std::endl;
	    return 0;
    }
    
    // Create a server object containing password and port.
    Server server(av[2], std::atoi(av[1]));
    server.setupSocketAndEvents();
    if (server.getServerStatus() == false) {
        std::cout << RED << "Error: Server: Cannot create the server!" << std::endl;
    }

    // Test
    std::cout << YELLOW << "\n----- Infos -----\n" << std::endl;
    std::cout << "Status: " << server.getServerStatus() << std::endl;
    std::cout << "Server socket: " << server.getServerFd() << std::endl;
    std::cout << "Server port: " << server.getServerPort() << std::endl;
    std::cout << std::endl;
}