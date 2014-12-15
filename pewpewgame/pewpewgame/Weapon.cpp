#include "Weapon.h"

Weapon::Weapon() {
	isStatic = true;
	z = 1.0f;

	shootTimer = 0.0f;
	rateOfFire = 1.0f;
	ammo = 100;
	max_ammo = 100;
}

void Weapon::changeWeapon(float scale_x, float scale_y, float rateOfFire, int ammo, int max_ammo)
{
	this->scale_x = scale_x;
	this->scale_y = scale_y;

	this->rateOfFire = rateOfFire;
	this->ammo = ammo;
	this->max_ammo = max_ammo;
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



