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
	else if (type == LASER) {
		scale_x = 5.0f;
		scale_y = 5.0f;
		damage = 20;
		sprite = SheetSprite(tex->weaponSpriteSheetTexture, 14, 7, 6);
	}
	else if (type == SMILEY_FACE) {
		scale_x = 2.0f;
		scale_y = 2.0f;
		damage = 20;
		sprite = SheetSprite(tex->smileyTexture, 1, 1, 0);
	}
	else if (type == FRUIT) {
		scale_x = 2.0f;
		scale_y = 2.0f;
		damage = 20;
		sprite = SheetSprite(tex->fruitSpriteSheetTexture, 176.0f/272.0f, 94.0f/300.0f, 17.0f/272.0f, 18.0f/300.0f);
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