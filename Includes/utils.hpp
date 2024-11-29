/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelotti <fbelotti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:44:26 by fbelotti          #+#    #+#             */
/*   Updated: 2024/11/29 16:12:49 by fbelotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Color.hpp"

#include <string>
#include <iostream>
#include <exception>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <csignal>

#include <vector>
#include <map>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>