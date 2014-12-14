#include "Enemy.h"

Enemy::Enemy() {}

void Enemy::Update(float elapsed) {
	Character::Update(elapsed);
}

void Enemy::FixedUpdate() {
	if (collidedRight) {
		setWalkLeft(0.8f);
	}

	if (collidedLeft) {
		setWalkRight(0.8f);
	}

	//check if enemy is dead
	if (hp <= 0) {
		y = 0.85f;
		x = -10.0f;
		hp = 2.0f;
	}

	Character::FixedUpdate();
}

void Enemy::Render() {
	Character::Render();
}