#include "Weapon.h"

Weapon::Weapon() {
	isStatic = true;
	z = 1.0f;

	shootTimer = 0.0f;
	rateOfFire = 1.0f;
	ammo = 100;
	max_ammo = 100;

	dropped = false;
}

void Weapon::changeWeapon(int type, float scale_x, float scale_y, float rateOfFire, int ammo, int max_ammo, int melee_damage)
{
	this->type = type;
	this->scale_x = scale_x;
	this->scale_y = scale_y;

	this->rateOfFire = rateOfFire;
	this->ammo = ammo;
	this->max_ammo = max_ammo;
	this->melee_damage = melee_damage;
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



