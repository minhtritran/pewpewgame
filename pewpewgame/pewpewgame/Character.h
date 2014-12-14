#pragma once

#include "Entity.h"

class Projectile;

class Character : public Entity {
public:
	Character();
	virtual void Update(float elapsed);
	virtual void FixedUpdate();
	virtual void Render();
	void jump();
	void setWalkRight();
	void setWalkLeft();
	void setIdle();

	void shoot(Projectile* projectile);

	int hp;
	bool face_left;
	bool isJumping;
};