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

//Game states
enum class GameState { PLAY, EXIT, OVER };

//Declare globally used objects extern so that any file that includes this header file will then see the same global objects
extern Player player;
extern Level level;
extern SDL_Renderer* renderer;
extern Texture tileTexture;
extern Texture enemyTexture;
extern Texture bulletTexture;
extern std::vector<Bullet> bulletVec;
extern std::vector<Enemy> enemyVec;
extern GameState gameState;
extern bool victory;

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
	void processInput(SDL_Rect camera);

	//Plays the game
	void playGame();

	//Restarts the game
	void restart();

	//Game over, returns true if user wants to restart
	bool gameOver();

	//The window that is being rendered to
	SDL_Window* _window;

	//The camera area
	SDL_Rect _camera;

};

#endif