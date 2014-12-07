#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SheetSprite.h"

class Entity {
public:
	Entity();
	~Entity();

	virtual void Update(float elapsed);
	virtual void Render();

	bool collidesWith(Entity* entity);
	void FixedUpdate();

	SheetSprite sprite;

	float x;
	float y;
	float width;
	float height;
	float rotation;
	
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float friction_x;
	float friction_y;
	float mass;
	bool isStatic;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
	bool isJumping;
};