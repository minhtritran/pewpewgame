#include "Weapon.h"

Weapon::Weapon(Textures* tex) : Entity(tex) {
	isStatic = true;
	z = 1.0f;

	shootTimer = 5.0f;
	rateOfFire = 1.0f;
	ammo = 100;
	max_ammo = 100;
	friction_x = 5.0f;
}

void Weapon::changeWeapon(int type)
{
	this->type = type;
	if (type == RAY_GUN) {
		scale_x = 1.5f;
		scale_y = 1.5f;

		rateOfFire = 1.0f;
		ammo = 150;
		max_ammo = 350;
		melee_damage = 0;

		droppable = true;
		bullet_type = PLASMA_BALL;
		sprite = SheetSprite(tex->weaponSpriteSheetTexture, 14, 7, 2);
	}
	else if (type == SWORD) {
		scale_x = 1.2f;
		scale_y = 0.4f;

		rateOfFire = 0.0f;
		ammo = 0;
		max_ammo = 0;
		melee_damage = 20;

		droppable = false;
		bullet_type = PLASMA_BALL;
		sprite = SheetSprite(tex->characterAnimationSpriteSheetTexture, 481.0f / 1320.0f, 987.0f / 1320.0f, 111.0f / 1320.0f, 33.0f / 1320.0f);
	}
	else if (type == MINI_GUN) {
		scale_x = 1.5f;
		scale_y = 1.5f;

		rateOfFire = 2.0f;
		ammo = 125;
		max_ammo = 200;
		melee_damage = 0;

		droppable = true;
		bullet_type = LASER;
		sprite = SheetSprite(tex->minigunTexture, 1, 1, 0);
	}
	else if (type == MACHINE_GUN) {
		scale_x = 1.5f;
		scale_y = 1.0f;

		rateOfFire = 1.5f;
		ammo = 150;
		max_ammo = 400;
		melee_damage = 0;

		droppable = true;
		bullet_type = SMILEY_FACE;
		sprite = SheetSprite(tex->gunsSpriteSheetTexture, 344.0f / 480.0f, 4.0f / 256.0f, 99.0f / 480.0f, 63.0f / 256.0f);
	}
	else if (type == SHOTGUN) {
		scale_x = 1.3f;
		scale_y = 0.5f;

		rateOfFire = 0.3f;
		ammo = 20;
		max_ammo = 40;
		melee_damage = 0;

		droppable = true;
		bullet_type = FRUIT;
		sprite = SheetSprite(tex->gunsSpriteSheetTexture, 96.0f / 480.0f, 212.0f / 256.0f, 95.0f / 480.0f, 33.0f / 256.0f);
	}
}

void Weapon::Update(float elapsed) {
	Entity::Update(elapsed);
	shootTimer += elapsed;
}
void Weapon::FixedUpdate() {
	//remove if fell
	if (y < -2.8f) {
		should_remove = true;
	}

	Entity::FixedUpdate();
}
void Weapon::Render(float elapsed) {
	Entity::Render(elapsed);
}

Projectile* Weapon::fire() {
	Projectile* projectile = new Projectile(tex);
	projectile->changeProjectile(bullet_type);
	return projectile;
}


