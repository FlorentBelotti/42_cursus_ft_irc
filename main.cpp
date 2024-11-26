/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:35:59 by fbelotti          #+#    #+#             */
/*   Updated: 2024/11/22 16:40:37 by fbelotti         ###   ########.fr       */
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
    Server server(av[2], std::stoi(av[1]));
}