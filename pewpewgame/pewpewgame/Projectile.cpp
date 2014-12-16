#include "Projectile.h"

Projectile::Projectile(Textures* tex) : Entity(tex) {
	width = 0.05f;
	height = 0.05f;

	velocity_x = 3.0f;

	damage = 1;

	damages_enemy = false;
	damages_player = false;
}

void Projectile::changeProjectile(int type)
{
	this->type = type;
	if (type == PLASMA_BALL) {
		scale_x = 7.5f;
		scale_y = 7.5f;
		damage = 20;
		sprite = SheetSprite(tex->weaponSpriteSheetTexture, 14, 7, 7);
	}
	
}

void Projectile::Update(float elapsed) {
	x += velocity_x * elapsed;
}

void Projectile::Render(float elapsed) {
	if (!should_remove) {
		Entity::Render(elapsed);
	}
}