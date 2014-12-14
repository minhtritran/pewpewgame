#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>

class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);

	float length();
	void normalize();

	float x;
	float y;
	float z;
};