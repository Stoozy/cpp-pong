#include <iostream>
#include <sstream>



#include <cmath>
#include <string>
#include <cstdlib>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Server.h"
#include "Client.h"

using namespace std;

// patch for to_string MingW
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

struct Ball {
	sf::RectangleShape shape;
    sf::Vector2f pos;
    float direction;

    Ball() {
        shape = sf::RectangleShape(sf::Vector2f(15, 15));
        shape.setPosition(sf::Vector2f(540, 360));
    }
    sf::Vector2f getPosition() {
        return shape.getPosition();
    }
    float getDirection() {
        return direction;
    }
    void setDirection(float d) {
        direction = d;
    }
    void setPosition(sf::Vector2f npos) {
        shape.setPosition(npos);
    }

    void reset() {
        shape.setPosition(sf::Vector2f(520, 340));
        direction = 30 + rand() % (60-30) +1;
    }
};

struct Paddle{
	sf::RectangleShape shape;
    sf::Vector2f pos;

    Paddle() {
        shape = sf::RectangleShape(sf::Vector2f(20, 150));
    }

    Paddle(sf::Vector2f dpos) {
        shape = sf::RectangleShape(sf::Vector2f(20, 150));
        shape.setPosition(dpos);
    }
    sf::Vector2f getPosition() {
        return shape.getPosition();
    }

    void setPosition(sf::Vector2f npos) {
        shape.setPosition(npos);
    }
};



float rad(float x) {
    return (x * 3.14 )/ 180;
}


int main(){
    
    srand(time(0));

	
    Paddle player_one = Paddle(sf::Vector2f(20, 50));
    Paddle ai = Paddle(sf::Vector2f(1030, 50));
    Ball b = Ball();

    // set initial direction to 30
    b.setDirection(30);
    int p1_score = 0, p2_score = 0;

    char r;
    cout << "Would you like to play online? (y/n)" << endl;
    cin >> r;
    if (r == 'n') {
        sf::RenderWindow window(sf::VideoMode(1080, 720), "Pong Game");

		// load font
		sf::Font font;
		font.loadFromFile("SourceSansPro-Regular.ttf");

		 while (window.isOpen()){
            sf::Event event;

            sf::Text pt1(patch::to_string(p1_score), font, 30);
            pt1.setPosition(1080 / 2 +10, 20);

            sf::Text pt2(patch::to_string(p2_score), font, 30);
            pt2.setPosition(1080 / 2 - 40, 20);


            // collisions
            if (b.shape.getGlobalBounds().intersects(ai.shape.getGlobalBounds())) 
                b.setDirection(360 - b.getDirection());
            
            if (b.shape.getGlobalBounds().intersects(player_one.shape.getGlobalBounds())) 
                b.setDirection((360 - b.getDirection()));
            
            if(b.getPosition().y < 10) 
                b.setDirection(180 - b.getDirection());

            if (b.getPosition().y > 680) 
                b.setDirection(180 - b.getDirection());	
            
            //update ai pos
		    //ai.setPosition(sf::Vector2f(ai.getPosition().x, b.getPosition().y));
            //if (ai.getPosition().y + 150 > 700) {
            //    ai.setPosition(sf::Vector2f(ai.getPosition().x, 560));
            //}

            // update ball pos
		    b.setPosition(sf::Vector2f(
		    	b.getPosition().x + (0.1 * sin(rad(b.getDirection()))), 
                b.getPosition().y - (0.1 * cos(rad(b.getDirection())))
            ));
            
            //update score 
            if (b.getPosition().x < 0) {
                p1_score++;
                b.reset();
            }
            if (b.getPosition().x > 1080) {
                p2_score++;
                b.reset();
            }
 
		    window.clear();
            while (window.pollEvent(event)){

                // movement
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)){
                    sf::Vector2f current_pos = player_one.getPosition();
                    if(current_pos.y<560)
		    			player_one.setPosition(sf::Vector2f(current_pos.x, current_pos.y+10));
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)){
                    sf::Vector2f current_pos = player_one.getPosition();
                    if(current_pos.y>10)
		    			player_one.setPosition(sf::Vector2f(current_pos.x, current_pos.y-10));
                }
                if (event.type == sf::Event::Closed)
                    window.close();

                //cout << "Paddles current pos is" << ":" << player_one.getPosition().y << endl;
		    }
            //cout << "Ball direction: " << b.getDirection() << " Ball x,y: " << b.getPosition().x << ", " << b.getPosition().y << endl;
            // draw line in the middle


            for (int i = 0; i <= 24; ++i) {
                sf::RectangleShape c(sf::Vector2f(15, 15));
                c.setPosition((1080/2) -15, i * 45);
                window.draw(c);
            }

            //  draw to screen
            window.draw(pt1);
            window.draw(pt2);
		    window.draw(player_one.shape);
		    window.draw(ai.shape);
		    window.draw(b.shape);

		    window.display();

		}
    }
    else {
        char r;
        std::cout << "Would you like to host? (y/n)" << std::endl;
        cin >> r;
        if (r == 'n') {
            std::string ip;
            Client c;
            std::cout << "[Client] Please enter servers ip: " << std::endl;
            cin >> ip;

            sf::Socket::Status s = c.connect(ip, 13);
            if (s != sf::Socket::Done) {
                std::cout << "[Client] Error occured connecting to " << ip << std::endl;
            }
            else {
                c.run();
            }
        }
        else {
			Server s(13);
			s.listen();
			s.run();

        }
    }
   
	return 0;
}
