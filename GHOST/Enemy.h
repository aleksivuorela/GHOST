#pragma once
#ifndef ENEMY_H
#define ENEMY_H
#include <stack>
#include <iostream>
#include <SDL.h>
#include <glm/glm.hpp>
#include <vector>

//States
enum class State { NORMAL, ALERT, EVASION };

class Enemy
{
public:
	//Initializes the variables
	Enemy(float enemyX, float enemyY);

	//Updates behavior based on the currently active state, returns true if out of health points -> delete
	bool update(float playerX, float playerY, SDL_Rect playerBox);

	//Idling
	void idle();

	//Attacking
	void attack(float playerX, float playerY, SDL_Rect playerBox);

	//Searching
	void search(SDL_Rect playerBox);

	//Shows the enemy on the screen
	void render(SDL_Rect& camera);

	//Get the enemy's collision box
	SDL_Rect getBox();

	//Get the currently active state
	State getCurrentState();

	//Moves the enemy towards the player and checks collision
	void move(glm::vec2 vec, SDL_Rect playerBox);

	//Checks the distance between the enemy and the player
	void checkDistance(float playerX, float playerY);

	//Moves the enemy to the player's last known position
	void moveToLastPos(SDL_Rect playerBox);

	//Draws a line between the enemy and the player with the Bresenham's line algorithm
	void drawLine(float x1, float y1, float x2, float y2);

	//Checks line of sight. If the line collides with a wall tile (there is an obstruction in sight), the enemy can't see the player. If it doesn't, the enemy can see the player.
	//Returns true if the enemy can see the player
	bool checkLoS();

	//Enemy takes given amount of damage; returns true if health points reach 0 -> enemy dies and is deleted from enemies vector
	bool takeDamage(int amount);

	//Move assignment operator
	//Any class put in a vector requires a copy assignment operator (or at least a move assignment operator in C++11)
	Enemy& operator=(const Enemy& other);

private:
	//Collision box of the enemy
	SDL_Rect _box;

	//Position of the enemy
	float _enemyX, _enemyY;

	//Last known position of the player
	float _lastPosX, _lastPosY;

	//Distance vector between the enemy and the player
	glm::vec2 _distVec;

	//Length of the distance vector
	float _distLen;

	//Distance vector between the enemy and the player's last known position
	glm::vec2 _lastPosVec;

	//Length of the lastPos vector
	float _lastPosLen;

	//Maximum axis velocity of the enemy
	const float ENEMY_VEL = 1;

	//Vision range of the enemy
	const float ENEMY_VISION_RANGE = 300;

	//Attack range of the enemy
	const float ENEMY_ATTACK_RANGE = 25;

	//The dimensions of the enemy
	const int ENEMY_WIDTH = 20;
	const int ENEMY_HEIGHT = 20;

	//A point in the line (of sight)
	SDL_Rect linePoint;

	//Vector of all the points in the line
	std::vector <SDL_Rect> points;

	//Health points of the enemy
	int _hp;

	//Attack damage of the enemy
	const int ENEMY_DMG = 10;

	//Starting hp
	const int ENEMY_HP = 30;

protected:
	//Stack of states; top element is the currently active state
	std::stack<State> stateStack;

};

#endif