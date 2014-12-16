#pragma once
#include "Entity.h"

enum type {RAY_GUN};

class Weapon : public Entity {
public:
	Weapon();

	void changeWeapon(int type, float scale_x, float scale_y, float rateOfFire, int ammo, int max_ammo);

	void Update(float elapsed);
	void FixedUpdate();
	void Render(float elapsed = 0.0f);
	
	float shootTimer;

	float rateOfFire;
	int ammo;
	int max_ammo;

	bool dropped;
	int type;
};