#pragma once

#include "Entity.h"

enum projectile_type {PLASMA_BALL, LASER, SMILEY_FACE, FRUIT};

class Projectile : public Entity {
public:
	Projectile(Textures* tex);

	void changeProjectile(int type);

	void Update(float elapsed);
	void Render(float elapsed = 0.0f);

	int type;

	int damage;

	bool damages_player;
	bool damages_enemy;;
};