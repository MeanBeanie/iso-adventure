#include <SFML/Graphics.hpp>
#include "world.hpp"
#include <iostream>

int main(){
  sf::RenderWindow window(sf::VideoMode(800, 450), "Strawberry");

	world::init();

	sf::Clock dClock;
	float delta;
  while(window.isOpen()){
		delta = dClock.getElapsedTime().asSeconds();
		dClock.restart();
    
		world::update();

		sf::Event event;
    while(window.pollEvent(event)){
      switch(event.type){
        case sf::Event::Closed: window.close(); break;
      }
    }

    window.clear(sf::Color(92, 147, 147, 255));
		for(std::vector<sf::Sprite> sprites : world::ground){
			for(sf::Sprite s : sprites){
				if(s.getPosition().x+16 > 0 && s.getPosition().x < 800 && s.getPosition().y+16 > 0 && s.getPosition().y < 450){
					window.draw(s);
				}
			}
		}
    window.display();
  }

  return 0;
}
