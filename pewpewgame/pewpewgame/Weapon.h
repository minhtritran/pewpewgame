#pragma once
#include "Entity.h"

class Weapon : public Entity {
public:
	Weapon();

	void Update(float elapsed);
	void FixedUpdate();
	void Render(float elapsed = 0.0f);
	
	float shootTimer;

	float rateOfFire;
	int ammo;
	int max_ammo;
};