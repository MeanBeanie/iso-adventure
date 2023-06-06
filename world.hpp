#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "PerlinNoise.hpp" // https://github.com/Reputeless/PerlinNoise

namespace world {
	sf::Texture tile;
	
	sf::Texture player_tex;
	sf::IntRect pRect;
	
	sf::Texture tree;

	float scale = 2.0f;
	int width = 30;
	int height = 30;
	const int tileW = 32;
	const int tileH = 32;
	std::vector< std::vector<sf::Sprite> > ground;

	// NOTE: reminder all of the perlin noise is from https://github.com/Reputeless/PerlinNoise
	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

	int hills(double noise){
		// chatgpt
    int remainder = (int)noise % 16;
    if (remainder <= 8) {
        return noise - remainder;
    } else {
        return noise + (16 - remainder);
    }
	}

	void init(){
		if(!tile.loadFromFile("sprites/tile.png")){
			std::cout << "grass tile texture [X]\n";
		}
		if(!player_tex.loadFromFile("sprites/player.png")){
			std::cout << "player textures [X]\n";
		}
		if(!tree.loadFromFile("sprites/tree-right.png")){
			std::cout << "tree texture [X]\n";
		}

		printf("w: %i\nh: %i\n", width, height);

		for(int y = 0; y < height; y++){
			std::vector<sf::Sprite> temp;
			for(int x = 0; x < width; x++){
				sf::Sprite s;
				s.setTexture(tile);
				s.setScale(scale, scale);

				int mapx = (x * (0.5f * tileW) + y * (-0.5f * tileW))+384;
				int mapy = (x * (0.25f * tileH) + y * (0.25f * tileH)) + hills(perlin.octave2D_01((x*0.01), (y*0.01), 4)*256);
				s.setPosition(mapx, mapy);
				temp.push_back(s);
			}
			ground.push_back(temp);
		}
	}
}
