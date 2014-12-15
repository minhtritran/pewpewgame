#include "Player.h"

Player::Player() {}

void Player::Update(float elapsed) {
	Character::Update(elapsed);
}

void Player::FixedUpdate() {
	Character::FixedUpdate();
}

void Player::Render(float elapsed) {
	Character::Render(elapsed);
}

