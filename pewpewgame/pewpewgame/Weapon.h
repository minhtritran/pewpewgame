#pragma once
#include "Entity.h"

enum type {RAY_GUN, SWORD, MINI_GUN};

class Weapon : public Entity {
public:
	Weapon();

	void changeWeapon(int type);

	void Update(float elapsed);
	void FixedUpdate();
	void Render(float elapsed = 0.0f);
	
	float shootTimer;

	float rateOfFire;
	int ammo;
	int max_ammo;

	bool droppable;
	int type;
	int melee_damage;

};