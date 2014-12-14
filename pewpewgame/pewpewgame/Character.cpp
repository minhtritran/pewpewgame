#include "Character.h"
#include "Projectile.h"

Character::Character() {
	hp = 1;
	face_left = false;
	isJumping = false;
}

void Character::Update(float elapsed) {
	Entity::Update(elapsed);
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
}

void Character::jump() {
	collidedBottom = false;
	isJumping = true;
	velocity_y = 3.5f;
}

void Character::setWalkRight(float multiple) {
	face_left = false;
	if (velocity_x < 0.0f)
		velocity_x = 0.0f;
	if (velocity_x < 2.0f)
		acceleration_x = 9.0f * multiple;
}
void Character::setWalkLeft(float multiple) {
	face_left = true;
	if (velocity_x > 0.0f)
		velocity_x = 0.0f;
	if (velocity_x > -2.0f)
		acceleration_x = -9.0f * multiple;
}
void Character::setIdle() {
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