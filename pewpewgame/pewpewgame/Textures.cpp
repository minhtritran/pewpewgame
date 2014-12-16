#include "Textures.h"

Textures::Textures() {
	brickSpriteSheetTexture = LoadTexture("resources/spriteTiles.png");
	characterSpriteSheetTexture = LoadTexture("resources/Sprites_Characters.png");
	characterAnimationSpriteSheetTexture = LoadTexture("resources/character_animations.png");
	fontTexture = LoadTexture("resources/pixel_font.png");
	weaponSpriteSheetTexture = LoadTexture("resources/sheet.png");
	minigunTexture = LoadTexture("resources/minigun.png");
	gunsSpriteSheetTexture = LoadTexture("resources/guns.png");
	smileyTexture = LoadTexture("resources/smiley.png");
	fruitSpriteSheetTexture = LoadTexture("resources/fruits.png");
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