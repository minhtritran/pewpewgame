#include "Projectile.h"

Projectile::Projectile() {
	//Make bullet start away from screen
	x = -2.0f;
	y = -2.0f;
	
	width = 0.05f;
	height = 0.05f;

	velocity_x = 3.0f;
}

void Projectile::Update(float elapsed) {
	x += velocity_x * elapsed;
}

void Projectile::Render(float elapsed) {
	if (visible) {
		Entity::Render(elapsed);
	}
}