#include "Bullet.h"
#include "MainGame.h"

Bullet::Bullet(float startX, float startY, glm::vec2 dir)
{
	//Initialize
	_lifeTime = 100;
	_bulletX = startX;
	_bulletY = startY;
	_direction = dir;
	_box.x = (int)startX;
	_box.y = (int)startY;
	_box.w = BULLET_WIDTH;
	_box.h = BULLET_HEIGHT;
}

void Bullet::render(SDL_Rect& camera)
{
	//If the bullet is on screen
	if (SDL_HasIntersection(&camera, &_box))
	{
		//Show the bullet
		bulletTexture.render(_box.x - camera.x, _box.y - camera.y);
	}
}

bool Bullet::update()
{
	//Move the bullet left or right
	_bulletX += _direction.x * BULLET_VEL;
	_box.x = (int)_bulletX;

	//Move the bullet up or down
	_bulletY += _direction.y * BULLET_VEL;
	_box.y = (int)_bulletY;

	//Decrease lifetime
	_lifeTime--;

	//If lifetime reaches 0, return true
	if (_lifeTime == 0)
	{
		return true;
	}
	return false;
}

Bullet& Bullet::operator=(const Bullet& other)
{
	_lifeTime = other._lifeTime;
	_bulletX = other._bulletX;
	_bulletY = other._bulletY;
	_direction = other._direction;
	_box.x = other._box.x;
	_box.y = other._box.y;
	_box.w = BULLET_WIDTH;
	_box.h = BULLET_HEIGHT;
	return *this;
}
