#include "Enemy.h"
#include "MainGame.h"

Enemy::Enemy()
{
	//Initialize the collision box
	_box.x = 0;
	_box.y = 0;
	_box.w = ENEMY_WIDTH;
	_box.h = ENEMY_HEIGHT;

	//Initialize the position
	_enemyX = 0;
	_enemyY = 0;

	//Initialize the vectors
	_distVec = glm::vec2(0, 0);
	_lastPosVec = glm::vec2(0, 0);

	//Initialize the length of the vectors
	_distLen = 0;
	_lastPosLen = 0;

	//Initialize the player's last known position
	_lastPosX = 0;
	_lastPosY = 0;

	//Initialize the currently active state
	stateStack.push(State::NORMAL);

}

void Enemy::update(float playerX, float playerY, SDL_Rect playerBox)
{
	//Check distance 
	checkDistance(playerX, playerY);

	//Draw line of sight
	drawLine(playerX, playerY, _enemyX, _enemyY);

	//Change behavior based on the currently active state
	switch (getCurrentState())
	{
	case State::NORMAL:
		//Normal state, just idling...
		idle();
		break;

	case State::ALERT:
		//Player spotted, attack him!
		attack(playerX, playerY, playerBox);
		break;

	case State::EVASION:
		//Player got away, search for him!
		search(playerBox);
		break;
	}
}

void Enemy::idle()
{
	//Player is in vision range and can be seen
	if (_distLen < ENEMY_VISION_RANGE && checkLoS())
	{
		//Player spotted, alert! 
		std::cout << "PLAYER SPOTTED, ATTACKING HIM!" << std::endl;
		// Insert a new element at the top of the stack (ALERT), above it's current top element
		stateStack.push(State::ALERT);
	}
	//Just idling...
	else
		std::cout << "I am just chillin..." << std::endl;
}

void Enemy::attack(float playerX, float playerY, SDL_Rect playerBox)
{
	//Player is out of vision range or can't be seen
	if (_distLen > ENEMY_VISION_RANGE || !checkLoS())
	{
		//Player got away, search for him!
		std::cout << "Enemy got away! Going to search for him!" << std::endl;
		//Write down last known location of the player
		_lastPosX = playerX;
		_lastPosY = playerY;
		// Insert a new element at the top of the stack (EVASION), above it's current top element
		stateStack.push(State::EVASION);
	}

	//Move towards the player if the player is out of attack range
	else if (_distLen > ENEMY_ATTACK_RANGE) 
	{
		move(_distVec, playerBox);
	}

	//Attack player if he's close enough
	else
	{
		std::cout << "I AM ATTACKING!" << std::endl;
	}
}

void Enemy::search(SDL_Rect playerBox)
{	
	//Move to last known position of the player
	moveToLastPos(playerBox);

	//Player is in vision range and can be seen -> found the player
	if (_distLen < ENEMY_VISION_RANGE && checkLoS())
	{ 
		//Removes the element on top of the stack -> state back to ALERT
		stateStack.pop();
	}
	else
		//Searching for the player
		std::cout << "Searching..." << std::endl;	
}

void Enemy::render(SDL_Rect& camera)
{
	//If the enemy is on screen
	if (SDL_HasIntersection(&camera, &_box))
	{
		//Show the enemy
		enemyTexture.render(_box.x - camera.x, _box.y - camera.y);
	}
}

SDL_Rect Enemy::getBox()
{
	return _box;
}

State Enemy::getCurrentState()
{
	//Top element of the stateStack is the currently active state
	return stateStack.top();
}

void Enemy::move(glm::vec2 vec, SDL_Rect playerBox)
{
	//Move the enemy left or right
	_enemyX += vec.x * ENEMY_VEL;
	_box.x = (int)_enemyX;

	//If the enemy went too far to the left or right or touched a wall or the player
	if (_enemyX < 0 || _enemyX + ENEMY_WIDTH > LEVEL_WIDTH || level.touchesWall(_box) || SDL_HasIntersection(&playerBox, &_box))
	{
		//Move back
		_enemyX -= vec.x * ENEMY_VEL;
		_box.x = (int)_enemyX;
	}

	//Move the enemy up or down
	_enemyY += vec.y * ENEMY_VEL;
	_box.y = (int)_enemyY;

	//If the enemy went too far up or down or touched a wall or the player
	if (_enemyY < 0 || _enemyY + ENEMY_HEIGHT > LEVEL_HEIGHT || level.touchesWall(_box) || SDL_HasIntersection(&playerBox, &_box))
	{
		//Move back
		_enemyY -= vec.y * ENEMY_VEL;
		_box.y = (int)_enemyY;
	}
}

void Enemy::setPosition(float posX, float posY)
{
	_enemyX = posX;
	_box.x = (int)_enemyX;
	_enemyY = posY;
	_box.y = (int)_enemyY;
}

void Enemy::checkDistance(float playerX, float playerY)
{
	//Create a vector in the direction that the enemy moves to (player's position)
	_distVec.x = playerX - _enemyX;
	_distVec.y = playerY - _enemyY;

	//Calculate the vector's length
	_distLen = glm::length(_distVec);

	//Divide each of the vector's (xy) components by it's length
	//This makes it's length equal to 1, direction stays the same
	//So now, adding the vector moves the enemy by one unit
	//Multiply that 1 unit by the enemy's speed, and now it's moving at the correct speed
	_distVec.x /= _distLen;
	_distVec.y /= _distLen;
}

void Enemy::moveToLastPos(SDL_Rect playerBox)
{
	//Create a vector in the direction that the enemy moves to (player's last known position)
	_lastPosVec.x = _lastPosX - _enemyX;
	_lastPosVec.y = _lastPosY - _enemyY;

	//Calculate the vector's length
	_lastPosLen = glm::length(_lastPosVec);

	//Divide each of the vector's (xy) components by it's length
	_lastPosVec.x /= _lastPosLen;
	_lastPosVec.y /= _lastPosLen;

	//Move to the player's last known position
	move(_lastPosVec, playerBox);
}

void Enemy::drawLine(float x1, float y1, float x2, float y2)
{
	points.clear();
	//Bresenham's line algorithm
	//Edited from: http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm
	bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	float dx = x2 - x1;
	float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	int maxX = (int)x2;

	for (int x = (int)x1; x<maxX; x++)
	{
		if (steep)
		{
			linePoint = { y, x, 10, 10 };
			points.push_back(linePoint);

		}
		else
		{
			linePoint = { x, y, 10, 10 };
			points.push_back(linePoint);
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

bool Enemy::checkLoS()
{
	//Go through all the points in the line
	for (int i = 0; i < points.size(); i++)
	{
		//Check if any of the points in the line collide with a wall tile (touchesWall returns true if there was collision)
		if (level.touchesWall(points[i]))
		{
			//There was collision so a wall is blocking sight; can't see the player -> return false
			return false;
		}
	}
	//No collision between the points in the line and the wall tiles in the level; can see the player -> return true
	return true;
}


