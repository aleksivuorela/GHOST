#pragma once
#ifndef MAINGAME_H
#define MAINGAME_H
#include <SDL.h>
#include <vector>
#include "Texture.h"
#include "Player.h"
#include "Level.h"
#include "Enemy.h"
#include "Bullet.h"

//Screen dimensions
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//Declare globally used objects extern so that any file that includes this header file will then see the same global objects
extern Level level;
extern SDL_Renderer* renderer;
extern Texture tileTexture;
extern Texture enemyTexture;
extern Texture bulletTexture;
extern std::vector<Bullet> bullets;

//Game states
enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	//Initializes variables
	MainGame();

	//Runs the game
	void run();

private:
	//Initializes SDL and creates window
	bool init();

	//Loads media
	bool loadMedia();

	//Frees media and shuts down SDL
	void close();

	//Loops the game until exit
	void gameLoop();

	//Processes input with SDL
	void processInput();

	//Draws the game
	void drawGame();

	//The window that is being rendered to
	SDL_Window* _window;

	//Game state
	GameState _gameState;

	//The camera area
	SDL_Rect _camera;

	//Player
	Player player;

	//Enemies
	Enemy enemies[TOTAL_ENEMIES];


};

#endif