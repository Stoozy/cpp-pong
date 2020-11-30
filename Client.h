#include <SFML/Network.hpp>
#ifndef CLIENT_H
#define CLIENT_H

class Client {
private:
    sf::TcpSocket socket_; 
	unsigned short port;

public:
	Client();
	~Client();

    sf::Socket::Status connect(sf::IpAddress addr, unsigned short port);
	void run();
};

#endif
