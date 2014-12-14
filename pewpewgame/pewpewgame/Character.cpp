#include "Character.h"
#include "Projectile.h"

Character::Character() {
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
	Entity::FixedUpdate();
}

void Character::Render() {
	Entity::Render();
}

void Character::jump() {
	collidedBottom = false;
	isJumping = true;
	velocity_y = 3.5f;
}

void Character::setWalkRight() {
	face_left = false;
	if (velocity_x < 0.0f)
		velocity_x = 0.0f;
	if (velocity_x < 2.0f)
		acceleration_x = 9.0f;
}
void Character::setWalkLeft() {
	face_left = true;
	if (velocity_x > 0.0f)
		velocity_x = 0.0f;
	if (velocity_x > -2.0f)
		acceleration_x = -9.0f;
}
void Character::setIdle() {
	velocity_x = 0.0f;
	acceleration_x = 0.0f;
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