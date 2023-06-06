#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>

#define TILEWIDTH 32
#define TILEHEIGHT 32

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
	const int width = 100;
	const int height = 100;

	void shiftWorld(int x, int y){
		for(int j = 0; j < tiles.size(); j++){
			for(int i = 0; i < tiles.at(j).size(); i++){
				tiles.at(j).at(i).changePos(x, y);
			}
		}
	}

	time_t lastSwitch;
	time_t curTime;

	class Player: public Object{
	public:
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
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){  
				pos.y -= speed*delta;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){  
				pos.y += speed*delta;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){  
				pos.x += speed*delta;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){  
				pos.x -= speed*delta;
			}

			if(pos.x < 0){
				worldOffset.x += 16;
				pos.x += 16;
				tiles.clear();
				for(int y = 0; y < height; y++){
					std::vector<Tile> temp;
					for(int x = 0; x < width; x++){
						int mapx = (x * (0.5f * TILEWIDTH) + y * (-0.5f * TILEWIDTH))+worldOffset.x;
						int mapy = (x * (0.25f * TILEHEIGHT) + y * (0.25f * TILEHEIGHT))+worldOffset.y;
						Tile t(mapx, mapy);
						temp.push_back(t);
					}
					tiles.push_back(temp);
				}
			}
			if(pos.x+32 > 800){
				worldOffset.x -= 16;
				pos.x -= 16;
				tiles.clear();
				for(int y = 0; y < height; y++){
					std::vector<Tile> temp;
					for(int x = 0; x < width; x++){
						int mapx = (x * (0.5f * TILEWIDTH) + y * (-0.5f * TILEWIDTH))+worldOffset.x;
						int mapy = (x * (0.25f * TILEHEIGHT) + y * (0.25f * TILEHEIGHT))+worldOffset.y;
						Tile t(mapx, mapy);
						temp.push_back(t);
					}
					tiles.push_back(temp);
				}
			}
			if(pos.y < 0){
				worldOffset.y += 16;
				pos.y += 16;
				tiles.clear();
				for(int y = 0; y < height; y++){
					std::vector<Tile> temp;
					for(int x = 0; x < width; x++){
						int mapx = (x * (0.5f * TILEWIDTH) + y * (-0.5f * TILEWIDTH))+worldOffset.x;
						int mapy = (x * (0.25f * TILEHEIGHT) + y * (0.25f * TILEHEIGHT))+worldOffset.y;
						Tile t(mapx, mapy);
						temp.push_back(t);
					}
					tiles.push_back(temp);
				}
			}
			if(pos.y+32 > 450){
				worldOffset.y -= 16;
				pos.y -= 16;
				tiles.clear();
				for(int y = 0; y < height; y++){
					std::vector<Tile> temp;
					for(int x = 0; x < width; x++){
						int mapx = (x * (0.5f * TILEWIDTH) + y * (-0.5f * TILEWIDTH))+worldOffset.x;
						int mapy = (x * (0.25f * TILEHEIGHT) + y * (0.25f * TILEHEIGHT))+worldOffset.y;
						Tile t(mapx, mapy);
						temp.push_back(t);
					}
					tiles.push_back(temp);
				}
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

		for(int y = 0; y < height; y++){
			std::vector<Tile> temp;
			for(int x = 0; x < width; x++){
				int mapx = (x * (0.5f * TILEWIDTH) + y * (-0.5f * TILEWIDTH))+worldOffset.x;
				int mapy = (x * (0.25f * TILEHEIGHT) + y * (0.25f * TILEHEIGHT))+worldOffset.y;
				Tile t(mapx, mapy);
				temp.push_back(t);
			}
			tiles.push_back(temp);
		}
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
