#include "Enemy.h"

Enemy::Enemy() {
	is_jumper = false;
}

void Enemy::Update(float elapsed) {
	Character::Update(elapsed);
}

void Enemy::FixedUpdate() {
	if (collidedRight && !isJumping) {
		setWalkLeft(0.8f);
	}

	if (collidedLeft && !isJumping) {
		setWalkRight(0.8f);
	}

	//check if enemy is dead
	if (hp <= 0) {
		die();
	}

	Character::FixedUpdate();
}

void Enemy::Render() {
	Character::Render();
}