#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"

class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float spriteWidth, float spriteHeight);
	SheetSprite(GLuint textureID, unsigned int spriteCountX, unsigned int spriteCountY, unsigned int index);

	void Draw(float width, float height, float x, float y, float rotation);
	void Draw(float width, float height, Matrix m1);

	GLuint textureID;
	float u;
	float v;
	float spriteWidth;
	float spriteHeight;

};

GLuint LoadTexture(const char *image_path);