#include "Character.h"

class Player : public Character {
public:
	Player();
	void Update(float elapsed);
	void FixedUpdate();
	void Render();
};