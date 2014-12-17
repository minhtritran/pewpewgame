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
		damage = 25;
		sprite = SheetSprite(tex->weaponSpriteSheetTexture, 14, 7, 7);
		penetration = 1;
		speed_multiplier = 1.0f;
	}
	else if (type == LASER) {
		scale_x = 5.0f;
		scale_y = 7.0f;
		damage = 10;
		sprite = SheetSprite(tex->weaponSpriteSheetTexture, 14, 7, 6);
		penetration = 3;
		speed_multiplier = 1.0f;
	}
	else if (type == SMILEY_FACE) {
		scale_x = 1.0f;
		scale_y = 1.0f;
		damage = 20;
		sprite = SheetSprite(tex->smileyTexture, 1, 1, 0);
		penetration = 1;
		speed_multiplier = 2.5f;
	}
	else if (type == FRUIT) {
		scale_x = 4.0f;
		scale_y = 4.0f;
		damage = 20;
		sprite = SheetSprite(tex->fruitSpriteSheetTexture, 176.0f/272.0f, 94.0f/300.0f, 17.0f/272.0f, 18.0f/300.0f);
		penetration = 7;
		speed_multiplier = 0.7f;
	}
}

void Projectile::Update(float elapsed) {
	x += velocity_x * elapsed * speed_multiplier;
}

void Projectile::Render(float elapsed) {
	if (!should_remove) {
		Entity::Render(elapsed);
	}
}