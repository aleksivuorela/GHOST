#include "Player.h"
#include "MainGame.h"

Player::Player()
{
	//Initialize the collision box
	_box.x = 0;
	_box.y = 0;
	_box.w = PLAYER_WIDTH;
	_box.h = PLAYER_HEIGHT;

	//Initialize the position
	_playerX = 0;
	_playerY = 0;
	_mouseX = 0;
	_mouseY = 0;

	//Initialize the velocity
	_velX = 0;
	_velY = 0;

	//Initialize the distance vector
	_distVec = glm::vec2(0, 0);

	//Initialize the length of the distance vector
	_distLen = 0;

}

void Player::handleEvent(SDL_Event& e, SDL_Rect camera)
{
	//If a mouse button was pressed
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		//If left mouse button was pressed
		if (e.button.button == SDL_BUTTON_LEFT)
		{ 
			//Get mouse position relative to the camera
			SDL_GetMouseState(&_mouseX, &_mouseY);
			_mouseX += camera.x;
			_mouseY += camera.y;
			//Shoot towards the mouse position
			shoot();
		}
	}

	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Wasd key was pressed, adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_w: _velY -= PLAYER_VEL; break;
		case SDLK_s: _velY += PLAYER_VEL; break;
		case SDLK_a: _velX -= PLAYER_VEL; break;
		case SDLK_d: _velX += PLAYER_VEL; break;
		}
	}

	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Wasd key was released, adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_w: _velY += PLAYER_VEL; break;
		case SDLK_s: _velY -= PLAYER_VEL; break;
		case SDLK_a: _velX += PLAYER_VEL; break;
		case SDLK_d: _velX -= PLAYER_VEL; break;
		}
	}
}

void Player::move(Enemy* enemies)
{
	//Move the player left or right
	_playerX += _velX;
	_box.x = (int)_playerX;
	
	//Go through the enemies
	for (int i = 0; i < TOTAL_ENEMIES; ++i)
	{
		//If the player went too far to the left or right or touched a wall or an enemy
		if (_playerX < 0 || _playerX + PLAYER_WIDTH > LEVEL_WIDTH || level.touchesWall(_box) || SDL_HasIntersection(&enemies[i].getBox(), &_box))
		{
			//Move back
			_playerX -= _velX;
			_box.x = (int)_playerX;
		}
	}

	//Move the player up or down
	_playerY += _velY;
	_box.y = (int)_playerY;

	//Go through the enemies
	for (int i = 0; i < TOTAL_ENEMIES; ++i)
	{
		//If the player went too far up or down or touched a wall or an enemy
		if (_playerY < 0 || _playerY + PLAYER_HEIGHT > LEVEL_HEIGHT || level.touchesWall(_box) || SDL_HasIntersection(&enemies[i].getBox(), &_box))
		{
			//Move back
			_playerY -= _velY;
			_box.y = (int)_playerY;
		}
	}	
}

void Player::setCamera(SDL_Rect& camera)
{
	//Center the camera over the player
	camera.x = (_box.x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (_box.y + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Player::shoot()
{
	//Create a vector in the direction that the bullet moves to (mouse cursor's position)
	_distVec.x = _mouseX - _playerX;
	_distVec.y = _mouseY - _playerY;

	//Calculate the vector's length
	_distLen = glm::length(_distVec);

	//Divide each of the vector's (xy) components by it's length
	_distVec.x /= _distLen;
	_distVec.y /= _distLen;

	//Add new bullet at the end of the bullets vector
	bullets.emplace_back(_playerX, _playerY, _distVec);
}

bool Player::loadTexture()
{
	if (!_playerTexture.loadFromFile("textures/dot.bmp"))
	{
		printf("Failed to load player texture!\n");
		return false;
	}

	return true;
}

void Player::free()
{
	//Free loaded images
	_playerTexture.free();
}

void Player::render(SDL_Rect& camera)
{
	//Show the player relative to the camera
	_playerTexture.render(_box.x - camera.x, _box.y - camera.y);
}

SDL_Rect Player::getBox()
{
	return _box;
}

float Player::getX()
{
	return _playerX;
}

float Player::getY()
{
	return _playerY;
}
