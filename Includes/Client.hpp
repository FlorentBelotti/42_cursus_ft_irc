/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:42:49 by fbelotti          #+#    #+#             */
/*   Updated: 2024/11/22 16:52:03 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

class Client {
    
    private:
    
    int         _clientFd;
    std::string _clientIp;
    
    public:
    
    // Constructor
    Client();

    // Getters
    int getClientFd() const;
    int getClientIp() const;

    // Setters
    void setClientFd(int clientFd);
    void setClientIp(std::string clientIp);
}