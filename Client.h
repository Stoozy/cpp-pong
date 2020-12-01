#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#ifndef CLIENT_H
#define CLIENT_H


class Client {

private:
    sf::TcpSocket socket_; 
	unsigned short port;

public:

    sf::Uint16 id_;
	Client();
	~Client();

    sf::Socket::Status connect(sf::IpAddress addr, unsigned short port);
    void wait();
    void send(float py, float bx, float by, bool hasBallData);
    sf::Packet recv();
};

#endif
