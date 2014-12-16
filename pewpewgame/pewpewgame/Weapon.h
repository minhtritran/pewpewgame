#pragma once
#include "Entity.h"
#include "Projectile.h"

enum weapon_type {RAY_GUN, SWORD, MINI_GUN, MACHINE_GUN};

class Weapon : public Entity {
public:
	Weapon(Textures* tex);

	void changeWeapon(int type);

	void Update(float elapsed);
	void FixedUpdate();
	void Render(float elapsed = 0.0f);

	Projectile* fire();
	
	float shootTimer;

	float rateOfFire;
	int ammo;
	int max_ammo;

	bool droppable;
	int type;
	int melee_damage;

	int bullet_type;

};