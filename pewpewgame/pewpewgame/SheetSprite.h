#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Matrix.h"

using namespace std;

class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float spriteWidth, float spriteHeight);
	SheetSprite(GLuint textureID, unsigned int spriteCountX, unsigned int spriteCountY, unsigned int index);

	void setAnimated(bool val, float fps, vector<float> framesU, vector<float> framesV);
	void Draw(float width, float height, Matrix m1, float elapsed = 0.0f);

	GLuint textureID;
	float u;
	float v;
	float spriteWidth;
	float spriteHeight;

	bool animated;
	float framesPerSecond;
	vector<float> framesU;
	vector<float> framesV;
	int animationIndex;
	float animationElapsed;

};

GLuint LoadTexture(const char *image_path);