#pragma once

#include <vector>
#include "Matrix.h"

using namespace std;

class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID);
	SheetSprite(unsigned int textureID, float u, float v, float spriteWidth, float spriteHeight);
	SheetSprite(GLuint textureID, unsigned int spriteCountX, unsigned int spriteCountY, unsigned int index);

	void setAnimated(bool val, float fps, vector<vector<float>> frames);
	void Draw(float width, float height, Matrix m1, float elapsed = 0.0f);

	GLuint textureID;
	float u;
	float v;
	float spriteWidth;
	float spriteHeight;

	bool animated;
	float framesPerSecond;
	vector<vector<float>> frames;
	int animationIndex;
	float animationElapsed;

	bool invert;
};