#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Textures {
public:
	Textures();

	GLuint characterSpriteSheetTexture;
	GLuint characterAnimationSpriteSheetTexture;
	GLuint brickSpriteSheetTexture;
	GLuint fontTexture;
	GLuint weaponSpriteSheetTexture;
	GLuint minigunTexture;
	GLuint gunsSpriteSheetTexture;
};

GLuint LoadTexture(const char *image_path);