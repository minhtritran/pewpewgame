#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SheetSprite{
public:
	SheetSprite();
	SheetSprite(GLuint textureID, unsigned int spriteCountX, unsigned int spriteCountY, unsigned int index);

	void Draw(float width, float height, float x, float y, float rotation);

	GLuint textureID;
	unsigned int spriteCountX;
	unsigned int spriteCountY;
	unsigned int index;
};

GLuint LoadTexture(const char *image_path);