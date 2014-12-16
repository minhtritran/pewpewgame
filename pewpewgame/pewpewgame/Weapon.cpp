#include "Weapon.h"

Weapon::Weapon() {
	isStatic = true;
	z = 1.0f;

	shootTimer = 0.0f;
	rateOfFire = 1.0f;
	ammo = 100;
	max_ammo = 100;

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
	}
	else if (type == SWORD) {
		scale_x = 1.2f;
		scale_y = 0.4f;

		rateOfFire = 0.0f;
		ammo = 0;
		max_ammo = 0;
		melee_damage = 20;

		droppable = false;
	}
	if (type == MINI_GUN) {
		scale_x = 1.5f;
		scale_y = 1.5f;

		rateOfFire = 2.0f;
		ammo = 250;
		max_ammo = 500;
		melee_damage = 0;

		droppable = true;
	}
}

void Weapon::Update(float elapsed) {
	Entity::Update(elapsed);
	shootTimer += elapsed;
}
void Weapon::FixedUpdate() {
	Entity::FixedUpdate();
}
void Weapon::Render(float elapsed) {
	Entity::Render(elapsed);
}



