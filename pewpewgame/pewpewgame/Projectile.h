#pragma once

#include "Entity.h"


class Projectile : public Entity {
public:
	Projectile();

	void changeProjectile(float scale_x, float scale_y, int damage);

	void Update(float elapsed);
	void Render(float elapsed = 0.0f);

	int damage;

	bool damages_player;
	bool damages_enemy;;
};