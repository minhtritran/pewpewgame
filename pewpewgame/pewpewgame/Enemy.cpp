#include "Enemy.h"

Enemy::Enemy() {}

void Enemy::Update(float elapsed) {
	Character::Update(elapsed);
}

void Enemy::FixedUpdate() {
	Character::FixedUpdate();
}

void Enemy::Render() {
	Character::Render();
}
