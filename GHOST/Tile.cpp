#include "Tile.h"
#include "MainGame.h"

Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	_box.x = x;
	_box.y = y;

	//Set the collision box
	_box.w = TILE_WIDTH;
	_box.h = TILE_HEIGHT;

	//Get the tile type
	_type = tileType;
}

void Tile::render(SDL_Rect& camera, SDL_Rect* tileClips)
{
	//If the tile is on screen
	if (SDL_HasIntersection(&camera, &_box))
	{
		//Show the tile
		tileTexture.render(_box.x - camera.x, _box.y - camera.y, &tileClips[_type]);
	}
}

int Tile::getType()
{
	return _type;
}

SDL_Rect Tile::getBox()
{
	return _box;
}

