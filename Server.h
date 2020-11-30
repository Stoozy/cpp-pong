#include <SFML/Network.hpp>
#ifndef SERVER_H
#define SERVER_H

class Server {
private:
	std::vector<sf::TcpSocket*> clients;
	sf::TcpListener listener;
	unsigned short port;

public:
	Server(unsigned short s);

	~Server();
	void run();
	void listen();
	void stop();
};

#endif
