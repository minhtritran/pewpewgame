#pragma once

#include "Entity.h"


class Projectile : public Entity {
public:
	Projectile();
	void Update(float elapsed);
	void Render();

	bool visible;
};