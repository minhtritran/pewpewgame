#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SheetSprite.h"
#include "Matrix.h"

#define FIXED_TIMESTEP 0.0166666f


class Entity {
public:
	Entity();
	~Entity();

	void setScale(float scale);
	void BuildMatrix();
	virtual void Update(float elapsed);
	virtual void FixedUpdate();
	virtual void Render(float elapsed = 0.0f);

	bool collidesWith(Entity* entity);
	

	SheetSprite sprite;

	float x;
	float y;
	float width;
	float height;
	float rotation;
	float scale_x;
	float scale_y;

	Matrix matrix;
	
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
};

float lerp(float v0, float v1, float t);