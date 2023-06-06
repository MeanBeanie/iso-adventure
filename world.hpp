#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>
#include <random>

#define TILEWIDTH 32
#define TILEHEIGHT 32

// from chat gpt
int getRandomNumber(int min, int max) {
    std::random_device rd;  // Create a random device to obtain a seed
    std::mt19937 engine(rd());  // Initialize the Mersenne Twister random number generator

    std::uniform_int_distribution<int> distribution(min, max);  // Create a uniform distribution

    return distribution(engine);  // Generate a random number within the specified range
}

namespace world {
	sf::Texture tile_right;
	sf::Texture tile_left;
	sf::Texture tile_right_borderless;
	sf::Texture tile_left_borderless;
	
	sf::Texture player_tex;
	sf::IntRect pRect;

	sf::Texture tree_right;
	sf::Texture tree_left;

	sf::Vector2i worldOffset(100, 100);

	class Object{
	public:
		sf::Sprite sprite;
		sf::Vector2f pos;

		void changePos(int nx, int ny){
			pos.x = nx;
			pos.y = ny;
		}
	};

	class Tile: public Object{
	public:
		Tile(){
			sprite.setTexture(tile_right);
			sprite.setScale(2.5f, 2.5f);
			sprite.setPosition(pos.x, pos.y);
		};
		Tile(int x, int y){
			pos.x = x;
			pos.y = y;
			sprite.setTexture(tile_right);
			sprite.setScale(2.5f, 2.5f);
			sprite.setPosition(pos.x, pos.y);
		}
	};
	
	std::vector< std::vector<Tile> > tiles;
	std::vector<Object> trees;
	const int width = 100;
	const int height = 100;

	void resetTiles(int a = 0){
		tiles.clear();
		for(int y = 0; y < height; y++){
			std::vector<Tile> temp;
			for(int x = 0; x < width; x++){
				int mapx = (x * (0.5f * TILEWIDTH) + y * (-0.5f * TILEWIDTH))+worldOffset.x;
				int mapy = (x * (0.25f * TILEHEIGHT) + y * (0.25f * TILEHEIGHT))+worldOffset.y;
				Tile t(mapx, mapy);
				if(a == 1){
					if(getRandomNumber(0, 100) == 27){
						Object tree;
						tree.sprite.setTexture(tree_right);
						tree.pos.x = mapx;
						tree.pos.y = mapy+16;
						tree.sprite.setPosition(mapx, mapy+16);
						tree.sprite.setScale(2.5f, 2.5f);
						trees.push_back(tree);
					}
				}
				temp.push_back(t);
			}
			tiles.push_back(temp);
		}
	}

	time_t lastSwitch;
	time_t curTime;

	class Player: public Object{
	public:
		enum Facing{
			Up = 0,
			UpLeft = 1,
			Left = 2,
			DownLeft = 3,
			Down = 4,
			DownRight = 5,
			Right = 6,
			UpRight = 7
		};

		Facing facing = Facing::DownLeft;

		Player(int x = 0, int y = 0){
			pos.x = x;
			pos.y = y;
			sprite.setTexture(player_tex);
			sprite.setScale(2.5f, 2.5f);
			sprite.setPosition(pos.x, pos.y);
			sprite.setTextureRect(pRect);
		}

		void move(float delta){
			int speed = 30;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
				speed = 400;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){  
				facing = Facing::Right;
				pos.x += speed*delta;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){  
				facing = Facing::Left;
				pos.x -= speed*delta;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
					facing = Facing::UpRight;
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
					facing = Facing::UpLeft;
				}
				else{
					facing = Facing::Up;
				}
				pos.y -= speed*delta;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){  
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
					facing = Facing::DownRight;
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
					facing = Facing::DownLeft;
				}
				else{
					facing = Facing::Down;
				}
				pos.y += speed*delta;
			}	

			switch(facing){
				case 0:{pRect.left = 32; pRect.top = 16; sprite.setTextureRect(pRect); break;}
				case 1:{pRect.left = 0; pRect.top = 32; sprite.setTextureRect(pRect); break;}
				case 2:{pRect.left = 32; pRect.top = 16; sprite.setTextureRect(pRect); break;}
				case 3:{pRect.left = 0; pRect.top = 0; sprite.setTextureRect(pRect); break;}
				case 4:{pRect.left = 32; pRect.top = 0; sprite.setTextureRect(pRect); break;}
				case 5:{pRect.left = 0; pRect.top = 16; sprite.setTextureRect(pRect); break;}
				case 6:{pRect.left = 32; pRect.top = 16; sprite.setTextureRect(pRect); break;}
				case 7:{pRect.left = 0; pRect.top = 32; sprite.setTextureRect(pRect); break;}
			};

			if(pos.x < 0){ 
				worldOffset.x += 16;
				pos.x += 16;    
				resetTiles();
			}
			if(pos.x+32 > 800){
				worldOffset.x -= 16;
				pos.x -= 16;    
				resetTiles();	
			}
			if(pos.y < 0){
				worldOffset.y += 16;
				pos.y += 16;
				resetTiles();	
			}
			if(pos.y+32 > 450){
				worldOffset.y -= 16;
				pos.y -= 16;    
				resetTiles();	
			}

			sprite.setPosition(pos.x, pos.y);
		}
	};

	void init(){
		time(&curTime);
		//tile_right.loadFromFile("sprites/tile.png");
		//tile_left.loadFromFile("sprites/left.png");
		tile_right.loadFromFile("sprites/tile-borderless.png");
		tile_left.loadFromFile("sprites/left-borderless.png");	
		
		player_tex.loadFromFile("sprites/player.png");
		pRect.left = 0;
		pRect.top = 0;
		pRect.width = 16;
		pRect.height = 16;

		tree_left.loadFromFile("sprites/tree-left.png");
		tree_right.loadFromFile("sprites/tree-right.png");

		resetTiles(1);	
	}
	
	// perspective is based on light direction, left means light source
	// is to the left of the block. 
	enum Perspective{
		Left = 0,
		Right,
		Back,
		Front
	};
	int perspective = Perspective::Right;

	void switchPerspective(int newPer){
		if(curTime - lastSwitch > 0.5){
			std::cout << "perp switched from " << perspective << " to " << newPer << "\n";
			perspective = newPer;
			switch(perspective){
				case Perspective::Right:{
					for(int j = 0; j < tiles.size(); j++){
						for(int i = 0; i < tiles.at(j).size(); i++){
							tiles.at(j).at(i).sprite.setTexture(tile_right);
						}
					}
					break;
				}
				case Perspective::Left:{
					for(int j = 0; j < tiles.size(); j++){
						for(int i = 0; i < tiles.at(j).size(); i++){
							tiles.at(j).at(i).sprite.setTexture(tile_left);
						}
					}
					break;
				}
				default:{
					for(std::vector<Tile> row : tiles){
						for(Tile t : row){
							t.sprite.setTexture(tile_right);
						}
					}
					break;
				}
			};
			time(&lastSwitch);
		}
	}

}
