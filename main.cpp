/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:35:59 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/29 16:01:35 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Server.hpp"

int	stopSignal = 0;

static void manageSignal(int signal)
{
	if (signal == SIGINT) {
        stopSignal = 1;
        std::cout << BLUE << "[SIGNAL]: received SIGINT signal" << std::endl;
        
    }
    else if (signal == SIGQUIT) {
		stopSignal = 1;
        std::cout << BLUE << "[SIGNAL]: received SIGQUIT signal" << std::endl;
    }
}

int main(int ac, char **av) {
    
    if (ac != 3) {
        std::cerr << RED << "USAGE: ./ircserv <port> <password>" << RESET_COLOR << std::endl;
        return 0;
    }

    Server server(av[2], std::atoi(av[1]));
    server.setupSocketAndEvents();
    
    if (server.getServerStatus() == false) {
        std::cout << RED << "SERVER: ERROR: Cannot create the server!" << std::endl;
        return (1);
    }
    else {
        signal(SIGINT, manageSignal);
        signal(SIGQUIT, manageSignal);
        server.serverLoop();
    }
    
    return 0;
}