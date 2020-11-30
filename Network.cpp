#include <SFML/Network.hpp>
#include <iostream>
#include "Server.h"
#include "Client.h"


Server::Server(unsigned short s) {
	this->port = s;
}

Server::~Server() {
	clients.clear();
}

void Server::listen() {
	if (listener.listen(this->port) == sf::Socket::Done) {
		std::cout << "[SERVER] Listening on port " << this->port << std::endl;

		while (clients.size() < 2) {
			sf::TcpSocket* client = new sf::TcpSocket;
			if (listener.accept(*client) == sf::Socket::Done)
			{
				clients.push_back(client);
				std::cout << "[SERVER] New Connection: " << client->getRemoteAddress() << std::endl;
			}
			else {
				delete client;
				std::cout << "[SERVER] Error occured accepting connection" << std::endl;

			}
		}
		std::cout <<"[SERVER] Both players have joined." << std::endl;
		return;
	}else std::cout << "[SERVER] Error occured binding socket." << std::endl;
};

void Server::run() {
	while (true)
	{
		for (int i = 0; i < clients.size(); i++) {
				std::string s = "Hello";
				sf::Packet p;
				p << s;
				clients[i]->send(p);
				std::cout << "[SERVER] Sent: " << s << " to " << clients[i]->getRemoteAddress() << std::endl;	
		}
	}
}

void Server::stop() 
{
		listener.close();
};


Client::Client() {
        // empty constructor	
}

Client::~Client() {
	this->socket_.disconnect();
}

sf::Socket::Status Client::connect(sf::IpAddress addr, unsigned short port) {
	return this->socket_.connect(addr, port);
}

void Client::run() {
	std::string s;
	sf::Packet p;
	while (true) {
			this->socket_.receive(p);
			p >> s;
			std::cout << "[Client] Server Said: " << s << std::endl;
	}
}

