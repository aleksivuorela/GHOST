#pragma once
#ifndef BULLET_H
#define BULLET_H
#include <glm/glm.hpp>
#include <SDL.h>

class Bullet
{
public:
	//Initializes the variables
	Bullet(float startX, float startY, glm::vec2 dir);

	//Shows the bullet on the screen
	void render(SDL_Rect& camera);

	//Updates the bullet's position and lifetime, returns true if the bullet went too far to the left or right or touched a wall or an enemy or when out of life -> delete
	bool update();

	//Move assignment operator
	//Any class put in a vector requires a copy assignment operator (or at least a move assignment operator in C++11)
	Bullet& operator=(const Bullet& other);

	//Checks collision against enemies
	bool touchesEnemy();

private:
	//Lifetime of the bullet, when it reaches 0 the bullet is deleted
	int _lifeTime;

	//Position of the bullet
	float _bulletX;
	float _bulletY;

	//Direction where the bullet is moving to (distance vector between the player and the mouse cursor)
	glm::vec2 _direction;

	//Collision box of the bullet
	SDL_Rect _box;

	//Maximum axis velocity of the bullet
	const float BULLET_VEL = 5;

	//The dimensions of the bullet
	const int BULLET_WIDTH = 10;
	const int BULLET_HEIGHT = 10;

	//Bullet damage
	const int BULLET_DMG = 10;

};

#endif

