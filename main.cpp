/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:35:59 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/01 00:47:48 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Server.hpp"

// Animation management

void    *serverIsRunningAnimation(void *arg) {
    
    Server* server = static_cast<Server*>(arg);
    
    std::cout << YELLOW << "Server: Is currently running";
    std::cout.flush();
    while (server->getServerStatus() == false) {
        for (int i = 0; i < 3; i++) {
            usleep(500000);
            std::cout << ".";
            std::cout.flush();
        }
        if (server->getServerStatus() == false) {
            usleep(500000);
            std::cout << "\b\b\b   \b\b\b";
            std::cout.flush();
        }
    }
    std::cout << std::endl;
    return NULL;
}

int main(int ac, char **av) {
    
    //Usage check.
    if (ac != 3) {
        std::cerr << RED << "Usage: ./ircserv <port> <password>" << RESET_COLOR << std::endl;
	    return 0;
    }
    
    // pthread_t ServerIsRunningThread;
    // pthread_create(&ServerIsRunningThread, NULL, serverIsRunningAnimation, NULL);
    
    // Create a server object containing password and port.
    Server server(av[2], std::atoi(av[1]));
    server.setupSocketAndEvents();
    if (server.getServerStatus() == false) {
        std::cout << RED << "Error: Server: Cannot create the server!" << std::endl;
    }
    
    // pthread_join(ServerIsRunningThread, NULL);
}