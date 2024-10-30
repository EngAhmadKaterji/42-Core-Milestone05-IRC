/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetChannel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 05:07:28 by root              #+#    #+#             */
/*   Updated: 2024/10/21 05:08:20 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string Server::getClientChannel(int clientSocket){
        std::map<std::string, Channel>::iterator channelIt = _channels.begin();
        for(; channelIt != _channels.end(); ++channelIt){
        if (channelIt->second.getClients().find(clientSocket) != channelIt->second.getClients().end())
            return channelIt->first;
        }
        return "";
}