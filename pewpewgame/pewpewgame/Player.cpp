#include "Player.h"

Player::Player(Textures* tex) : Character(tex) {
	//Player static sprites
	sprite = SheetSprite(tex->characterSpriteSheetTexture, 272.0f / 2048.0f, 0.0f / 2048.0f, 132.0f / 2048.0f, 165.0f / 2048.0f);
	sprite_face_right = SheetSprite(tex->characterSpriteSheetTexture, 918.0f / 2048.0f, 1323.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f);
	sprite_face_left = SheetSprite(tex->characterSpriteSheetTexture, 1039.0f / 2048.0f, 1490.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f);
	sprite_jump_right = SheetSprite(tex->characterAnimationSpriteSheetTexture, 840.0f / 1320.0f, 1137.0f / 1320.0f, 120.0f / 1320.0f, 165.0f / 1320.0f);
	sprite_jump_left = SheetSprite(tex->characterAnimationSpriteSheetTexture, 243.0f / 1320.0f, 165.0f / 1320.0f, 120.0f / 1320.0f, 165.0f / 1320.0f);

	//Player animated sprites
	vector<vector<float>> frames_walk_right;
	vector<vector<float>> frames_walk_left;
	frames_walk_right.push_back({ 918.0f / 2048.0f, 1323.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	frames_walk_right.push_back({ 918.0f / 2048.0f, 662.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	frames_walk_right.push_back({ 917.0f / 2048.0f, 495.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	frames_walk_left.push_back({ 1039.0f / 2048.0f, 1490.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	frames_walk_left.push_back({ 1038.0f / 2048.0f, 996.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	frames_walk_left.push_back({ 1038.0f / 2048.0f, 829.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });

	//walk right animation
	animation_walk_right = SheetSprite(tex->characterSpriteSheetTexture);
	animation_walk_right.setAnimated(true, 8.0f, frames_walk_right);

	//walk left animation
	animation_walk_left = SheetSprite(tex->characterSpriteSheetTexture);
	animation_walk_left.setAnimated(true, 8.0f, frames_walk_left);

	sprite_in_use = SPRITE;
}

void Player::Update(float elapsed) {
	Character::Update(elapsed);
}

void Player::FixedUpdate() {
	Character::FixedUpdate();
	
}

void Player::Render(float elapsed) {
	Character::Render(elapsed);
}

