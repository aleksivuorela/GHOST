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

	//If the bullet went too far to the left or right or touched a wall or an enemy
	if (_bulletX < 0 || _bulletX + BULLET_WIDTH > LEVEL_WIDTH || level.touchesWall(_box) || touchesEnemy())
	{
		//Return true -> delete this bullet
		return true;
	}

	//Move the bullet up or down
	_bulletY += _direction.y * BULLET_VEL;
	_box.y = (int)_bulletY;

	//If the bullet went too far up or down or touched a wall or an enemy
	if (_bulletY < 0 || _bulletY + BULLET_HEIGHT > LEVEL_HEIGHT || level.touchesWall(_box) || touchesEnemy())
	{
		//Return true -> delete this bullet
		return true;
	}

	//Decrease lifetime
	_lifeTime--;

	//If lifetime reaches 0, return true -> delete this bullet
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

bool Bullet::touchesEnemy()
{
	//Go through enemies
	for (int i = 0; i < enemyVec.size(); ++i)
	{
		//If the collision box touches an enemy
		if (SDL_HasIntersection(&_box, &enemyVec[i].getBox()))
		{
			//Deal damage to the enemy
			enemyVec[i].takeDamage(BULLET_DMG);

			//Return true if an enemy was touched
			return true;
		}
	}
	//If no enemies were touched return false
	return false;
}
