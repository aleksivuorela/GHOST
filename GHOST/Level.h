#pragma once
#ifndef LEVEL_H
#define LEVEL_H
#include <SDL.h>
#include "Tile.h"

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 2;

//The different tile sprites
const int TILE_STD = 0;
const int TILE_WALL = 1;

class Level
{
public:
	Level();

	//Sets tiles from tile map
	bool setTiles();

	//Checks collision box against set of tiles
	bool touchesWall(SDL_Rect box);

	//Shows the level on the screen
	void render(SDL_Rect& camera);

private:
	//The level tiles
	Tile* tiles[TOTAL_TILES];

	//Clip rectangles that define the tile sprites
	SDL_Rect _tileClips[TOTAL_TILE_SPRITES];
};

#endif

