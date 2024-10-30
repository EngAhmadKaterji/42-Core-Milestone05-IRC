/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FindClientSocket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 05:07:25 by root              #+#    #+#             */
/*   Updated: 2024/10/21 05:08:23 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::findClientSocketByNick(const std::string &nickname) {
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second.getNickName() == nickname) {
            return it->first;
        }
    }
    return -1;
}
