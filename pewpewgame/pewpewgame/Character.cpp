#include "Character.h"
#include "Projectile.h"

Character::Character(Textures* tex) : Entity(tex) {
	hp = 10;
	face_left = false;
	isJumping = false;
	jumpTimer = 0.0f;
	equipTimer = 1.0f;
	weapon = NULL;
}

Character::~Character() {
}

void Character::Update(float elapsed) {
	if (weapon) {
		if (face_left)
			weapon->x = x - 0.05f;
		else
			weapon->x = x + 0.05f;
		weapon->y = y - 0.05f;
	}
	
	Entity::Update(elapsed);
	jumpTimer += elapsed;
	equipTimer += elapsed;
	
}

void Character::FixedUpdate() {

	if (y < -2.8f) {
		die();
	}

	if (collidedBottom) {
		isJumping = false;
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

	if (weapon)
		weapon->sprite.invert = false;
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

	if (weapon)
		weapon->sprite.invert = true;
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

Projectile* Character::shoot() {
	if (weapon && weapon->shootTimer > (0.2f / weapon->rateOfFire) && weapon->ammo > 0) {
		Projectile* projectile = weapon->fire();
		if (face_left)
			projectile->x = weapon->x - 0.1f;
		else
			projectile->x = weapon->x + 0.1f;

		projectile->y = weapon->y + genRandomNumber(-0.05f, 0.05f);
		projectile->velocity_y = genRandomNumber(-0.25f, 0.25f);
		projectile->rotation = 0.0f;
		projectile->velocity_x = 3.5f;
		if (face_left)
			projectile->velocity_x *= -1;
		
		weapon->ammo--;
		weapon->shootTimer = 0.0f;
		
		return projectile;
	}
	return NULL;
}

int Character::melee() {
	if (weapon) {
		if (weapon->melee_damage != 0)
			return weapon->melee_damage;
		else
			return 0;
	}
	else {
		return 5;
	}
}

void Character::equip(Weapon* newWeapon) {
	if (equipTimer > 0.5f) {
		if (weapon) {
			if (weapon->type == newWeapon->type) {
				weapon->ammo = weapon->ammo + newWeapon->ammo;
				if (weapon->ammo > weapon->max_ammo)
					weapon->ammo = weapon->max_ammo;
				newWeapon->should_remove = true;
			}
			else {
				if (velocity_x == 0.0f && velocity_y == 0.0f) {
					weapon->gravity_affected = true;
					weapon->sprite.invert = false;
					weapon->velocity_y = 4.0f;
					if (face_left)
						weapon->velocity_x = 1.5f;
					else
						weapon->velocity_x = -1.5f;
					weapon = NULL;
					weapon = newWeapon;
					weapon->gravity_affected = false;
					if (face_left)
						weapon->sprite.invert = true;
				}
				
			}
		}
		else {
			weapon = newWeapon;
			weapon->gravity_affected = false;
		}
		equipTimer = 0.0f;
	}
}

void Character::die() {
	if (weapon) {
		if (weapon->droppable) {
			weapon->gravity_affected = true;
			weapon->velocity_y = 1.5f;
			weapon->sprite.invert = false;
		}
		else {
			weapon->should_remove = true;
		}
		weapon = NULL;		
	}
	should_remove = true;
}