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

        sf::Uint16 _id  = 0;
		while (clients.size() < 2) {
			sf::TcpSocket* client = new sf::TcpSocket;
			if (listener.accept(*client) == sf::Socket::Done)
			{
                sf::Packet p;
                p << _id;
                client->send(p);
                this->selector.add(*client);
				clients.push_back(client);
				std::cout << "[SERVER] New Connection: " << client->getRemoteAddress() << std::endl;

                _id++;
			}
			else {
				delete client;
				std::cout << "[SERVER] Error occured accepting connection" << std::endl;

			}
		}
		std::cout <<"[SERVER] Both players have joined." << std::endl;
        sf::Packet p;
        bool sig = 1; 
        p << sig;
        clients[0]->send(p);
		return;
	}else std::cout << "[SERVER] Error occured binding socket." << std::endl;
};

void Server::run() {
    std::cout << "[SERVER] Game started " << std::endl;
	while (true)
	{
        // process goes here
        // TODO: implement server main process       
        for(int i=0; i<clients.size(); i++){
                sf::Packet p;
                if(clients[i]->receive(p) != sf::Socket::Done){
                    //std::cout << "[SERVER] Error occured receiving data" << std::endl;
                } else{
                    //std::cout << "[SERVER] Received data from " << clients[i]->getRemoteAddress() << std::endl;
                }
                if(i == 0){
                    if(clients[1]->send(p) != sf::Socket::Done){
                        //std::cout << "[SERVER] Error occured sending data" << std::endl;
                    }else{
                        //std::cout << "[SERVER] Send successful " << std::endl;
                    }
                }else{
                    if(clients[0]->send(p) != sf::Socket::Done){
                        //std::cout << "[SERVER] Error occured sending data" << std::endl;
                    }else{
                        //std::cout << "[SERVER] Send successful " << std::endl;
                    }
                }
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

void Client::send(float py, float bx, float by,  bool hasBallData ){
    // send
    sf::Packet p;
    if(hasBallData)
    {
        p << py << bx << by;
    }
    else
    {
        p << py;
    }
    if(this->socket_.send(p) == sf::Socket::Done){
        //std::cout << "[CLIENT] Send successful " << std::endl;
    }
    else
    {
        //std::cout << "[CLIENT] Send unsuccessful " << std::endl;
    }

}

void Client::wait(){
    sf::Packet p;
    bool start = 0;
    while(!start){
        if(this->socket_.receive(p) == sf::Socket::Done){
            // received signal
            p >> start;
        }
    }
    return;
    
}

sf::Packet Client::recv(){
    sf::Packet p;
    if(this->socket_.receive(p) == sf::Socket::Done){
        return p;
    }
}

sf::Socket::Status Client::connect(sf::IpAddress addr, unsigned short port) {
    sf::Packet p;
    sf::Socket::Status s = this->socket_.connect(addr, port);
    // handshake
    while(this->socket_.receive(p) != sf::Socket::Done){
        // do nothing
    }
    // at this point, the client must have gotten an id
    if(p >> this->id_) std::cout << "[CLIENT] Received id: " << this->id_ << std::endl;
    
     
}


