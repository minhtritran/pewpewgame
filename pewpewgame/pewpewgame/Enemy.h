#pragma once

#include "Character.h"

class Enemy : public Character {
public:
	Enemy();
	void Update(float elapsed);
	void FixedUpdate();
	void Render(float elapsed = 0.0f);

	bool is_jumper;
	float aiShootTimer;
	bool near_player;
};