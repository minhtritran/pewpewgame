#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include "Entity.h"
#include "Projectile.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "PewRunner.h"
#include "PewShooter.h"
#include "Weapon.h"

#define MAX_TIMESTEPS 6
#define MAX_BRICKS 200
#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 14
#define TILE_SIZE 0.2f

using namespace std;

class SideScroller{
public:
	SideScroller();
	~SideScroller();

	void Init();
	void Update(float elapsed);
	void FixedUpdate();
	void Render(float elapsed);
	bool UpdateAndRender();

	void resetGame();
	void buildLevel(string filename);
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);
	void placeEntity(string& type, float placeX, float placeY);
	void RenderLevel();
	bool isSolid(unsigned char tile);
	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
	float SideScroller::checkPointForGridCollisionX(float x, float y);
	float SideScroller::checkPointForGridCollisionY(float x, float y);
	void SideScroller::doLevelCollisionX(Entity* entity);
	void SideScroller::doLevelCollisionY(Entity* entity);
	

private:
	bool done;
	int state;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	GLuint characterSpriteSheetTexture;
	GLuint characterAnimationSpriteSheetTexture;
	GLuint brickSpriteSheetTexture;
	GLuint fontTexture;
	GLuint weaponSpriteSheetTexture;

	SheetSprite projectile_raygun_bullet_sprite;
	 
	SheetSprite weapon_raygun_sprite;

	SheetSprite player_sprite;
	SheetSprite player_sprite_face_right;
	SheetSprite player_sprite_face_left;
	SheetSprite player_sprite_jump_right;
	SheetSprite player_sprite_jump_left;
	vector<vector<float>> player_frames_walk_right;
	vector<vector<float>> player_frames_walk_left;

	SheetSprite enemy_sprite;
	SheetSprite enemy_sprite_face_right;
	SheetSprite enemy_sprite_face_left;
	SheetSprite enemy_sprite_jump_right;
	SheetSprite enemy_sprite_jump_left;
	vector<vector<float>> enemy_frames_walk_right;
	vector<vector<float>> enemy_frames_walk_left;

	float gravity_x;
	float gravity_y;

	vector<Entity*> entities;
	vector<Projectile*> projectiles;
	vector<Enemy*> enemies;
	Player* player;

	int brickIndex;
	Entity bricks[MAX_BRICKS];

	

	unsigned int mapWidth;
	unsigned int mapHeight;

	unsigned char** levelData;

	
	

	float shootTimer;
	float enemySpawnTimer;

	Mix_Chunk* gunshot;
	Mix_Chunk* jump;
	Mix_Music* music;

};

float lerp(float v0, float v1, float t);
void DrawText(int textureID, string text, float size, float spacing, float r, float g, float b, float a);
GLuint LoadTexture(const char *image_path);