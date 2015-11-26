#include "MainGame.h"
#include "Tile.h"
#include <stdio.h>
#include <iostream>
#include <SDL_image.h>

//Level
Level level;

//Renderer
SDL_Renderer* renderer = NULL;

//Tile texture
Texture tileTexture;

//Enemy texture
Texture enemyTexture;

//Bullet texture
Texture bulletTexture;

//Bullets vector
std::vector<Bullet> bullets;

MainGame::MainGame()
{
	//Initialize
	_window = NULL;
	_gameState = GameState::PLAY;
	_camera.x = 0;
	_camera.y = 0;
	_camera.w = SCREEN_WIDTH;
	_camera.h = SCREEN_HEIGHT;
}

void MainGame::run()
{
	//Start up SDL and create window
	if (!init())
	{
		close();
		printf("Failed to initialize!\n");
		std::cout << "Press any key to continue...";
		int tmp;
		std::cin >> tmp;
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			close();
			printf("Failed to load media!\n");
			std::cout << "Press any key to continue...";
			int tmp;
			std::cin >> tmp;
		}
		else
		{
			//Enter the game loop
			gameLoop();
		}
	}
}

bool MainGame::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		_window = SDL_CreateWindow("GHOST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize enemies starting positions
				enemies[0].setPosition(300, 200);
				enemies[1].setPosition(700, 700);
				enemies[2].setPosition(900, 500);
			}
		}
	}

	return success;
}

bool MainGame::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load player texture
	if (!player.loadTexture())
	{
		success = false;
	}

	//Load tile texture
	if (!tileTexture.loadFromFile("textures/tiles.png"))
	{
		printf("Failed to load tile texture!\n");
		success = false;
	}

	//Load enemy texture
	if (!enemyTexture.loadFromFile("textures/enemy.bmp"))
	{
		printf("Failed to load enemy texture!\n");
		success = false;
	}

	//Load bullet texture
	if (!bulletTexture.loadFromFile("textures/bullet.bmp"))
	{
		printf("Failed to load bullet texture!\n");
		success = false;
	}

	//Load tile map
	if (!level.setTiles())
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	return success;
}

void MainGame::close()
{
	//Free loaded images
	player.free();
	tileTexture.free();
	enemyTexture.free();

	//Destroy window    
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(_window);
	_window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void MainGame::gameLoop()
{
	//Will loop until _gameState is set to EXIT
	while (_gameState != GameState::EXIT)
	{
		//Process input
		processInput();
		//Draw the game
		drawGame();
	}

	//Free resources and close SDL
	close();
}

void MainGame::processInput()
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			_gameState = GameState::EXIT;
		}

		//Handle input for the player
		player.handleEvent(e);
	}
}

void MainGame::drawGame()
{
	//Move player
	player.move(enemies);
	player.setCamera(_camera);

	//Update enemies
	for (int i = 0; i < TOTAL_ENEMIES; i++)
	{
		enemies[i].update(player.getX(), player.getY(), player.getBox());
	}

	//Update bullets
	for (int i = 0; i < bullets.size();)
	{
		if (bullets[i].update() == true)
		{
			//Little trick to easily delete element from vector; swap with the last element of the vector
			bullets[i] = bullets.back();
			bullets.pop_back();
		}
		//Only increment i if bullet wasn't deleted (if i++ is inside for(), skips over the bullet that is in bullets.back())
		else
		{
			i++;
		}
	}

	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	//Render level
	level.render(_camera);

	//Render player
	player.render(_camera);

	//Render enemies
	for (int i = 0; i < TOTAL_ENEMIES; i++)
	{
		enemies[i].render(_camera);
	}

	//Render bullets
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i].render(_camera);
	}

	//Update screen
	SDL_RenderPresent(renderer);
}
