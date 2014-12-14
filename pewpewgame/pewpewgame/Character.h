#pragma once

#include "Entity.h"

class Projectile;

class Character : public Entity {
public:
	Character();
	virtual void Update(float elapsed);
	virtual void FixedUpdate();
	virtual void Render(float elapsed = 0.0f);
	void jump();
	void setWalkRight(float multiple);
	void setWalkLeft(float multiple);
	void setIdle();

	void shoot(Projectile* projectile);

	int hp;
	bool face_left;
	bool isJumping;
};