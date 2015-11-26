#include "Level.h"
#include <fstream>

Level::Level()
{
}

bool Level::setTiles()
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("levels/level1.map");
	if (map) 
	{
		// The map file exists, and is open for input
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If there was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType);
			}
			//If the tile type isn't recognized 
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}
			//Move to next tile spot
			x += TILE_WIDTH;

			//If gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}
		//Clip the sprite sheet
		if (tilesLoaded)
		{
			_tileClips[TILE_STD].x = 0;
			_tileClips[TILE_STD].y = 0;
			_tileClips[TILE_STD].w = TILE_WIDTH;
			_tileClips[TILE_STD].h = TILE_HEIGHT;

			_tileClips[TILE_WALL].x = 80;
			_tileClips[TILE_WALL].y = 0;
			_tileClips[TILE_WALL].w = TILE_WIDTH;
			_tileClips[TILE_WALL].h = TILE_HEIGHT;
		}
	}
	//If the map couldn't be loaded
	else 
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}

bool Level::touchesWall(SDL_Rect box)
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if (tiles[i]->getType() == TILE_WALL)
		{
			//If the collision box touches the wall tile
			if (SDL_HasIntersection(&box, &tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}

void Level::render(SDL_Rect& camera) 
{
	//Show the level relative to the camera
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		tiles[i]->render(camera, _tileClips);
	}
}




