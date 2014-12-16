#pragma once

#include "Entity.h"
#include "Weapon.h"

class Projectile;

class Character : public Entity {
public:
	Character(Textures* tex);
	~Character();
	virtual void Update(float elapsed);
	virtual void FixedUpdate();
	virtual void Render(float elapsed = 0.0f);
	bool jump();
	void setWalkRight(float multiple);
	void setWalkLeft(float multiple);
	void setIdle();

	Projectile* shoot();
	int melee();
	void equip(Weapon* weapon);
	void die();

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
	float equipTimer;

	Weapon* weapon;
};