#include "SheetSprite.h"

SheetSprite::SheetSprite() {
}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float spriteWidth, float spriteHeight)
	: textureID(textureID), u(u), v(v), spriteWidth(spriteWidth), spriteHeight(spriteHeight)
{}

SheetSprite::SheetSprite(GLuint textureID, unsigned int spriteCountX, unsigned int spriteCountY, unsigned int index)
	: textureID(textureID) {
	
	if (spriteCountX == 0)
		spriteCountX = 1;
	if (spriteCountY == 0)
		spriteCountY = 1;

	u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	spriteWidth = 1.0f / (float)spriteCountX;
	spriteHeight = 1.0f / (float)spriteCountY;
}

void SheetSprite::setAnimated(bool val, float fps, vector<float> framesU, vector<float> framesV)
{
	animated = val;
	framesPerSecond = fps;
	this->framesU = framesU;
	this->framesV = framesV;
	animationIndex = 0;
	animationElapsed = 0.0f;
}

void SheetSprite::Draw(float width, float height, Matrix m1, float elapsed) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glMultMatrixf(m1.ml);

	GLfloat quad[] = { -width * 0.5f, height * 0.5f, -width * 0.5f, -height * 0.5f, width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	//Check for animation
	if (animated)
	{
		animationElapsed += elapsed;
		if (animationElapsed > 1.0 / framesPerSecond)
		{
			animationIndex++;
			animationElapsed = 0.0f;
			if (animationIndex > (framesU.size() - 1))
			{
				animationIndex = 0;
			}
		}
		if (animationIndex >= 0) {
			GLfloat quadUVs[] = { framesU[animationIndex], framesV[animationIndex], framesU[animationIndex], framesV[animationIndex] + height, framesU[animationIndex] + width, framesV[animationIndex] + height, framesU[animationIndex] + width, framesV[animationIndex] };
			glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		}
		
	}
	else
	{
		GLfloat quadUVs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	}
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);

	return textureID;
}