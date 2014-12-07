#include "Entity.h"

class Player : public Entity {
public:
	Player();
	void Update(float elapsed);
	void Render();
	void jump();
	void setWalkRight();
	void setWalkLeft();
	void setIdle();

	bool face_left;
};