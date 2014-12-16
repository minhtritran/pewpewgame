#include "Enemy.h"

Enemy::Enemy(Textures* tex) : Character(tex) {
	is_jumper = false;

	//Enemy static sprites
	sprite = SheetSprite(tex->characterSpriteSheetTexture, 405.0f / 2048.0f, 167.0f / 2048.0f, 132.0f / 2048.0f, 161.0f / 2048.0f);
	sprite_face_right = SheetSprite(tex->characterSpriteSheetTexture, 1160.0f / 2048.0f, 827.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f);
	sprite_face_left = SheetSprite(tex->characterSpriteSheetTexture, 916.0f / 2048.0f, 995.0f / 2048.0f, 120.0f / 2048.0f, 161.0f / 2048.0f);
	sprite_jump_right = SheetSprite(tex->characterAnimationSpriteSheetTexture, 122.0f / 1320.0f, 476.0f / 1320.0f, 120.0f / 1320.0f, 161.0f / 1320.0f);
	sprite_jump_left = SheetSprite(tex->characterAnimationSpriteSheetTexture, 721.0f / 1320.0f, 483.0f / 1320.0f, 120.0f / 1320.0f, 161.0f / 1320.0f);

	//Enemy animated sprites
	vector<vector<float>> frames_walk_right;
	vector<vector<float>> frames_walk_left;
	frames_walk_right.push_back({ 1160.0f / 2048.0f, 827.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	frames_walk_right.push_back({ 1158.0f / 2048.0f, 0.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	frames_walk_right.push_back({ 795.0f / 2048.0f, 1820.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	frames_walk_left.push_back({ 916.0f / 2048.0f, 995.0f / 2048.0f, 120.0f / 2048.0f, 161.0f / 2048.0f });
	frames_walk_left.push_back({ 1040.0f / 2048.0f, 1163.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	frames_walk_left.push_back({ 796.0f / 2048.0f, 831.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });

	//walk right animation
	animation_walk_right = SheetSprite(tex->characterSpriteSheetTexture);
	animation_walk_right.setAnimated(true, 8.0f, frames_walk_right);

	//walk left animation
	animation_walk_left = SheetSprite(tex->characterSpriteSheetTexture);
	animation_walk_left.setAnimated(true, 8.0f, frames_walk_left);
}

void Enemy::Update(float elapsed) {
	Character::Update(elapsed);
	aiShootTimer += elapsed;
	aiMeleeTimer += elapsed;
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

void Enemy::Render(float elapsed) {
	Character::Render(elapsed);
}