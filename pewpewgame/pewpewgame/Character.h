#pragma once

#include "Entity.h"

class Projectile;

class Character : public Entity {
public:
	Character();
	virtual void Update(float elapsed);
	virtual void FixedUpdate();
	virtual void Render(float elapsed = 0.0f);
	bool jump();
	void setWalkRight(float multiple);
	void setWalkLeft(float multiple);
	void setIdle();

	void shoot(Projectile* projectile);


	SheetSprite sprite_face_right;
	SheetSprite sprite_face_left;
	SheetSprite sprite_jump_right;
	SheetSprite sprite_jump_left;
	SheetSprite animation_walk_right;
	SheetSprite animation_walk_left;

	int hp;
	bool face_left;
	bool isJumping;
	float jumpTimer;
};