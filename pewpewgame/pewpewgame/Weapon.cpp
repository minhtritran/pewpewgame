#include "Weapon.h"

Weapon::Weapon() {
	isStatic = true;
	z = 1.0f;

	shootTimer = 0.0f;
	rateOfFire = 1.0f;

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



