/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:44:26 by fbelotti          #+#    #+#             */
/*   Updated: 2024/12/20 00:06:18 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Define

#define CLIENT_NB 10

// Custom

#include "Color.hpp"

// Include .cpp

#include <string>
#include <iostream>
#include <exception>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <iomanip>
#include <sstream>

// Containers

#include <vector>
#include <map>

// Include .c

#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <poll.h>
#include <netdb.h>