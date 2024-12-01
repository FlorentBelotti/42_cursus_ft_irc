/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:35:59 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/01 21:30:49 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Server.hpp"

int main(int ac, char **av) {
    
    if (ac != 3) {
        std::cerr << RED << "Usage: ./ircserv <port> <password>" << RESET_COLOR << std::endl;
        return 0;
    }

    Server server(av[2], std::atoi(av[1]));
    server.setupSocketAndEvents();
    if (server.getServerStatus() == false) {
        std::cout << RED << "Error: Server: Cannot create the server!" << std::endl;
    }

    pthread_t dashboardThread;
    if (pthread_create(&dashboardThread, NULL, Server::startDashboard, &server) != 0) {
        std::cerr << RED << "Error: Cannot create dashboard thread!" << RESET_COLOR << std::endl;
        return 1;
    }

    if (pthread_join(dashboardThread, NULL) != 0) {
        std::cerr << RED << "Error: Cannot join dashboard thread!" << RESET_COLOR << std::endl;
        return 1;
    }

    // Test
    // std::cout << YELLOW << "\n----- Infos -----\n" << std::endl;
    // std::cout << "Status: " << server.getServerStatus() << std::endl;
    // std::cout << "Server socket: " << server.getServerFd() << std::endl;
    // std::cout << "Server port: " << server.getServerPort() << std::endl;
    // std::cout << std::endl;
}