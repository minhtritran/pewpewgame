#include "Projectile.h"

Projectile::Projectile() {
	//Make bullet start away from screen
	x = -2.0f;
	y = -2.0f;
	
	width = 0.05f;
	height = 0.05f;

	velocity_x = 3.0f;

	damage = 1;
}

void Projectile::changeProjectile(float scale_x, float scale_y, int damage)
{
	this->scale_x = scale_x;
	this->scale_y = scale_y;
	this->damage = damage;
}

void Projectile::Update(float elapsed) {
	x += velocity_x * elapsed;
}

void Projectile::Render(float elapsed) {
	if (!should_remove) {
		Entity::Render(elapsed);
	}
}