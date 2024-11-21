/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:31:52 by fbelotti          #+#    #+#             */
/*   Updated: 2024/11/19 16:39:07 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.hpp"
#include <string>
#include <iostream>
#include <exception>

class Server {
    
    private:
    
    std::string _pswd;
    int         _port;
    Server();

    public:
    
    Server(std::string pswd, int port);
}