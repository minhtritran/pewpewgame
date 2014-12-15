#include "Character.h"
#include "Projectile.h"

Character::Character() {
	hp = 1;
	face_left = false;
	isJumping = false;
}

void Character::Update(float elapsed) {
	Entity::Update(elapsed);
	jumpTimer += elapsed;
}

void Character::FixedUpdate() {
	if (collidedBottom) {
		isJumping = false;
	}

	//falls to death
	if (y < -2.5f) {
		hp -= 10;
	}

	Entity::FixedUpdate();
}

void Character::Render(float elapsed) {
	Entity::Render(elapsed);
	if (sprite_in_use == SPRITE_FACE_RIGHT)
		sprite_face_right.Draw(width, height, matrix, elapsed);
	else if (sprite_in_use == SPRITE_FACE_LEFT)
		sprite_face_left.Draw(width, height, matrix, elapsed);
	else if (sprite_in_use == SPRITE_JUMP_RIGHT)
		sprite_jump_right.Draw(width, height, matrix, elapsed);
	else if (sprite_in_use == SPRITE_JUMP_LEFT)
		sprite_jump_left.Draw(width, height, matrix, elapsed);
	else if (sprite_in_use == ANIMATION_WALK_RIGHT)
		animation_walk_right.Draw(width, height, matrix, elapsed);
	else if (sprite_in_use == ANIMATION_WALK_LEFT)
		animation_walk_left.Draw(width, height, matrix, elapsed); 
	
}

bool Character::jump() {
	if (!isJumping && jumpTimer > 0.25f) {
		if (face_left)
			sprite_in_use = SPRITE_JUMP_LEFT;
		else
			sprite_in_use = SPRITE_JUMP_RIGHT;

		collidedBottom = false;
		isJumping = true;
		velocity_y = 3.5f;
		jumpTimer = 0.0f;
		return true;
	}
	else
		return false;
}

void Character::setWalkRight(float multiple) {
	
	if (isJumping)
		sprite_in_use = SPRITE_JUMP_RIGHT;
	else
		sprite_in_use = ANIMATION_WALK_RIGHT;
	face_left = false;
	if (velocity_x < 0.0f)
		velocity_x = 0.0f;
	if (velocity_x < 2.0f)
		acceleration_x = 9.0f * multiple;
}
void Character::setWalkLeft(float multiple) {
	if (isJumping)
		sprite_in_use = SPRITE_JUMP_LEFT;
	else
		sprite_in_use = ANIMATION_WALK_LEFT;	
	face_left = true;
	if (velocity_x > 0.0f)
		velocity_x = 0.0f;
	if (velocity_x > -2.0f)
		acceleration_x = -9.0f * multiple;
}
void Character::setIdle() {
	if (face_left)
		sprite_in_use = SPRITE_FACE_LEFT;
	else
		sprite_in_use = SPRITE_FACE_RIGHT;

	velocity_x = 0.0f;
	acceleration_x = 0.0f;
	sprite.animated = false;
}

void Character::shoot(Projectile* projectile) {
	projectile->visible = true;
	projectile->x = x;
	projectile->y = y;
	projectile->rotation = 0.0f;
	projectile->velocity_x = 3.5f;
	if (face_left)
		projectile->velocity_x *= -1;
}