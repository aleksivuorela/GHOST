#pragma once
#ifndef TILE_H
#define TILE_H
#include <SDL.h>

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	void render(SDL_Rect& camera, SDL_Rect* tileClips);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

private:
	//The attributes of the tile
	SDL_Rect _box;

	//The tile type
	int _type;
};

#endif

