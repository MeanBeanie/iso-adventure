#include <SFML/Graphics.hpp>
#include "world.hpp"
#include <iostream>

int main(){
  sf::RenderWindow window(sf::VideoMode(800, 450), "Strawberry");

	world::init();

	world::Player player(100, 100);

	sf::Clock dClock;
	float delta;
  while(window.isOpen()){
		delta = dClock.getElapsedTime().asSeconds();
		dClock.restart();
		time(&world::curTime);
    
		sf::Event event;
    while(window.pollEvent(event)){
      switch(event.type){
        case sf::Event::Closed: window.close(); break;
      }
    }
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			int newPer = world::perspective;
			newPer++;
			if(newPer > world::Perspective::Front){
				newPer = world::Perspective::Left;
			}
			world::switchPerspective(newPer);
			continue;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			int newPer = world::perspective;
			newPer--;
			if(newPer < 0){
				newPer = world::Perspective::Front;
			}
			world::switchPerspective(newPer);
			continue;
		}

		player.move(delta);

    window.clear(sf::Color(92, 147, 147, 255));
		for(int y = 0; y < world::height; y++){
			for(int x = 0; x < world::width; x++){
				sf::Vector2f tPos = world::tiles[y][x].sprite.getPosition();
				if(tPos.x+16 > 0 && tPos.x < 800 && tPos.y+16 > 0 && tPos.y < 450){
					window.draw(world::tiles[y][x].sprite);
				}
			}
		}
		for(world::Object obj : world::trees){
			window.draw(obj.sprite);
		}
		window.draw(player.sprite);
    window.display();
  }

  return 0;
}
