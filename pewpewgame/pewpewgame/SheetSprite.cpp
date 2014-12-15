#include "SheetSprite.h"

SheetSprite::SheetSprite() {
	animated = 0;
	invert = false;
}

SheetSprite::SheetSprite(unsigned int textureID)
	: textureID(textureID)
{
	animated = 0;
	invert = false;
}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float spriteWidth, float spriteHeight)
	: textureID(textureID), u(u), v(v), spriteWidth(spriteWidth), spriteHeight(spriteHeight)
{
	animated = 0;
	invert = false;
}

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

	animated = 0;
	invert = false;
}

void SheetSprite::setAnimated(bool val, float fps, vector<vector<float>> frames)
{
	animated = val;
	framesPerSecond = fps;
	this->frames = frames;
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
			if (animationIndex > (frames.size() - 1))
			{
				animationIndex = 0;
			}
		}
		u = frames[animationIndex][0];
		v = frames[animationIndex][1];
		spriteWidth = frames[animationIndex][2];
		spriteHeight = frames[animationIndex][3];
	}

		
	if (!invert) {
		GLfloat quadUVs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	}
	else {
		GLfloat quadUVs[] = { u + spriteWidth, v, u + spriteWidth, v + spriteHeight, u, v + spriteHeight, u, v };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	}
		

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	
}