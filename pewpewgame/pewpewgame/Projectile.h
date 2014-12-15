#pragma once

#include "Entity.h"


class Projectile : public Entity {
public:
	Projectile();
	void Update(float elapsed);
	void Render(float elapsed = 0.0f);

	bool visible;

	int damage;
};