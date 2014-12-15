#pragma once

#include "Character.h"

class Enemy : public Character {
public:
	Enemy();
	void Update(float elapsed);
	void FixedUpdate();
	void Render();

	bool is_jumper;
};