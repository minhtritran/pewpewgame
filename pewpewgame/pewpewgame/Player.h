#pragma once

#include "Character.h"

class Player : public Character {
public:
	Player(Textures* tex);
	void Update(float elapsed);
	void FixedUpdate();
	void Render(float elapsed = 0.0f);
};