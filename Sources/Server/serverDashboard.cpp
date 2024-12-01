/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverDashboard.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 22:00:10 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/01 22:00:34 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

// Dashboard

void Server::printDashboard() const {

    std::cout << "\033[2J\033[1;1H";
    const int colWidth = 5;
    std::cout << std::setw(colWidth) << YELLOW << "SERVER INFOS" << RESET_COLOR << " | "
              << std::setw(colWidth) << YELLOW << "PORT " << RESET_COLOR << _serverPort << " | "
              << std::setw(colWidth) << YELLOW << "SOCKET " << RESET_COLOR << _serverFd << " | "
              << std::setw(colWidth) << YELLOW << "STATE " << RESET_COLOR << (_serverStatus ? GREEN : RED) << (_serverStatus ? "UP" : "DOWN") << RESET_COLOR << std::endl;
              
    // std::cout << "CLIENT | IP | SOCK | STATE | HOUR" << std::endl;
    // for (size_t i = 0; i < _clients.size(); ++i) {
    //     const Client& client = _clients[i];
    //     std::time_t connectionTime = client.getConnectionTime();
    //     std::tm* tm = std::localtime(&connectionTime);
    //     char timeStr[100];
    //     std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", tm);

    //     std::cout << client.getName() << " | " << client.getIP() << " | " << client.getSocket() 
    //               << " | " << (client.isConnected() ? "connected" : "disconnected") 
    //               << " | " << timeStr << std::endl;
    // }
}

void* Server::startDashboard(void* arg) {
    Server* server = static_cast<Server*>(arg);
    while (server->getServerStatus()) {
        server->printDashboard();
        sleep(10);
    }
    return NULL;
}

// Animation management

void    *Server::serverIsRunningAnimation(void *arg) {
    
    Server* server = static_cast<Server*>(arg);
    
    std::cout << YELLOW << "Server: Is currently running";
    std::cout.flush();
    while (server->getServerStatus() == true) {
        for (int i = 0; i < 3; i++) {
            usleep(500000);
            std::cout << ".";
            std::cout.flush();
        }
        if (server->getServerStatus() == true) {
            usleep(500000);
            std::cout << "\b\b\b   \b\b\b";
            std::cout.flush();
        }
    }
    std::cout << std::endl;
    return NULL;
}