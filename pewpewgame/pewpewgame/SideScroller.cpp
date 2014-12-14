#include "SideScroller.h"

enum GameState { STATE_TITLE, STATE_GAME, STATE_GAMEOVER };

SideScroller::SideScroller() {
	Init();
	
	state = STATE_TITLE;
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	gravity_x = 0.0f;
	gravity_y = -9.8f;

	projectileIndex = 0;
	shootTimer = 0.1f;

	brickSpriteSheetTexture = LoadTexture("resources/spriteTiles.png");
	characterSpriteSheetTexture = LoadTexture("resources/Sprites_Characters.png");
	fontTexture = LoadTexture("resources/pixel_font.png");
	bulletSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 3);
	buildLevel();

	gunshot = Mix_LoadWAV("resources/gunshot.wav");
	jump = Mix_LoadWAV("resources/jump.wav");
	//music = Mix_LoadMUS("resources/music.wav");
	if (Mix_PlayMusic(music, -1) < 0) {
		cout << "Error";
	}
	
}

SideScroller::~SideScroller() {


	SDL_Quit();
}

void SideScroller::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.66, 2.66, -2.0, 2.0, -2.0, 2.0);
}

void SideScroller::Update(float elapsed) {
	if (state == STATE_TITLE)
	{
		return;
	}
	else if (state == STATE_GAME)
	{
		if (enemySpawnTimer > 1.0f && enemyIndex < 8) {
			SheetSprite enemySprite = SheetSprite(characterSpriteSheetTexture, 405.0f / 2048.0f, 167.0f / 2048.0f, 132.0f / 2048.0f, 161.0f / 2048.0f);
			enemies[enemyIndex].sprite = enemySprite;
			enemies[enemyIndex].y = 0.85f;
			enemies[enemyIndex].x = -10.0f;
			enemies[enemyIndex].setScale(2.5f);
			enemies[enemyIndex].friction_x = 3.0f;
			enemies[enemyIndex].hp = 2;
			enemies[enemyIndex].setWalkLeft(0.8f);
			entities.push_back(&enemies[enemyIndex]);
			enemyIndex++;
			enemySpawnTimer = 0.0f;
		}

		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->Update(elapsed);
		}

		for (size_t i = 0; i < MAX_PROJECTILES; i++) {
			projectiles[i].Update(elapsed);
		}

		shootTimer += elapsed;
		enemySpawnTimer += elapsed;
		jumpTimer += elapsed;
	}
}

void SideScroller::FixedUpdate() {
	if (state == STATE_TITLE)
	{
		return;
	}
	else if (state == STATE_GAME)
	{
		for (size_t i = 0; i < entities.size(); i++) {
			if (!entities[i]->isStatic) {
				entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
				entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
			}
			entities[i]->FixedUpdate();

			entities[i]->y += entities[i]->velocity_y * FIXED_TIMESTEP;
			//do Y collisions
			if (!entities[i]->isStatic) {
				for (size_t j = 0; j < entities.size(); j++) {
					if (entities[i]->collidesWith(entities[j]) && entities[i] != entities[j]) {
						float yPenetration = fabs(fabs(entities[j]->y - entities[i]->y) - entities[i]->height / 2.0f - entities[j]->height / 2.0f);
						if (entities[i]->y > entities[j]->y) {
							entities[i]->y += yPenetration + 0.001f;
							entities[i]->collidedBottom = true;
						}
						else if (entities[i]->y < entities[j]->y) {
							entities[i]->y -= yPenetration + 0.001f;
							entities[i]->collidedTop = true;
						}
					}
				}

			}
			doLevelCollisionY(entities[i]);

			entities[i]->x += entities[i]->velocity_x * FIXED_TIMESTEP;
			//do X collisions
			if (!entities[i]->isStatic) {
				for (size_t j = 0; j < entities.size(); j++) {
					if (entities[i]->collidesWith(entities[j]) && entities[i] != entities[j]) {
						float xPenetration = fabs(fabs(entities[j]->x - entities[i]->x) - entities[i]->width / 2.0f - entities[j]->width / 2.0f);
						if (entities[i]->x > entities[j]->x) {
							entities[i]->x += xPenetration + 0.001f;
							entities[i]->collidedLeft = true;
						}
						else if (entities[i]->x < entities[j]->x) {
							entities[i]->x -= xPenetration + 0.001f;
							entities[i]->collidedRight = true;
						}
					}
				}
			}
			doLevelCollisionX(entities[i]);
		}


		for (int i = 0; i < MAX_ENEMIES; i++) {
			//enemy gets hit
			for (int k = 0; k < MAX_PROJECTILES; k++) {
				if (checkPointForGridCollisionX(projectiles[k].x, projectiles[k].y) != 0)
					projectiles[k].visible = false;
				if (enemies[i].collidesWith(&projectiles[k]) && projectiles[k].visible) {
					projectiles[k].visible = false;
					enemies[i].hp--;
				}
			}
		}

		//check if player is dead
		if (player->hp <= 0)
			state = STATE_GAMEOVER;
	}
}

void SideScroller::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (state == STATE_TITLE)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Matrix tempMatrix;
		tempMatrix.identity();
		tempMatrix.m[3][0] = -0.9f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(fontTexture, "Press SPACE to start", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (state == STATE_GAME)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushMatrix();
		Matrix tempMatrix;
		tempMatrix.identity();
		tempMatrix.m[3][0] = -2.4f;
		tempMatrix.m[3][1] = 1.85f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(fontTexture, "HP: " + to_string(player->hp) + "/5", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glPopMatrix();

		float translateX = -player->x;
		float translateY = -player->y;

		//scrolling boundaries
		if (translateY > (float)(mapHeight * TILE_SIZE / 2.0f) - 2.00f)
			translateY = (float)(mapHeight * TILE_SIZE / 2.0f) - 2.00f;
		else if (translateY < -(float)(mapHeight * TILE_SIZE / 2.0f) + 2.00f)
			translateY = -(float)(mapHeight * TILE_SIZE / 2.0f) + 2.00f;

		if (translateX >(float)(mapWidth * TILE_SIZE / 2.0f) - 2.66f)
			translateX = (float)(mapWidth * TILE_SIZE / 2.0f) - 2.66f;
		else if (translateX < (float)(-1.0f * mapWidth * TILE_SIZE / 2.0f) + 2.66f)
			translateX = (float)(-1.0f * mapWidth * TILE_SIZE / 2.0f) + 2.66f;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		tempMatrix.identity();
		tempMatrix.m[3][0] = translateX;
		tempMatrix.m[3][1] = translateY;
		glMultMatrixf(tempMatrix.ml);
		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->Render();
		}

		for (size_t i = 0; i < MAX_PROJECTILES; i++) {
			projectiles[i].Render();
		}
		RenderLevel();
	}
	else if (state = STATE_GAMEOVER)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Matrix tempMatrix;
		tempMatrix.identity();
		tempMatrix.m[3][0] = -0.8f;
		glMultMatrixf(tempMatrix.ml);
		if (player->hp <= 0)
			DrawText(fontTexture, "Game Over! You Died!", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		else
			DrawText(fontTexture, "Game Over! You Won!", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	SDL_GL_SwapWindow(displayWindow);
}

bool SideScroller::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (state == STATE_TITLE)
					state = STATE_GAME;
			}
		}
	}

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (state == STATE_TITLE)
	{
		//do nothing
	}
	else if (state == STATE_GAME)
	{
		if (keys[SDL_SCANCODE_UP]) {
			if (!player->isJumping && jumpTimer > 0.25f) {
				Mix_PlayChannel(-1, jump, 0);
				player->jump();
				jumpTimer = 0.0f;
			}

		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 918.0f / 2048.0f, 1323.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f);
			player->sprite = playerSprite;
			player->setWalkRight(1.0f);
		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 1039.0f / 2048.0f, 1490.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f);
			player->sprite = playerSprite;
			player->setWalkLeft(1.0f);
		}
		else {
			player->setIdle();
		}
		if (keys[SDL_SCANCODE_SPACE]) {
			if (shootTimer > 0.15f) {

				Mix_PlayChannel(-1, gunshot, 0);
				projectiles[projectileIndex].sprite = bulletSprite;

				player->shoot(&projectiles[projectileIndex]);
				
				projectileIndex++;
				if (projectileIndex > MAX_PROJECTILES - 1) {
					projectileIndex = 0;
				}
				shootTimer = 0.0f;
				

			}
		}
	}

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();
	return done;
}

void SideScroller::buildLevel() {
	ifstream infile("resources/Level1.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			readEntityData(infile);
		}
	}
}

bool SideScroller::readHeader(ifstream& stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}
bool SideScroller::readLayerData(ifstream& stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}

			}
		}
	}
	return true;
}
bool SideScroller::readEntityData(ifstream& stream) {
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			
			float placeX = (atoi(xPosition.c_str()) + 8.0f - mapWidth * 64.0f / 2.0f) / 64.0f * TILE_SIZE;
			float placeY = (atoi(yPosition.c_str()) - 8.0f - mapHeight * 64.0f / 2.0f) / 64.0f * -TILE_SIZE;

			
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void SideScroller::placeEntity(string& type, float placeX, float placeY) {
	if (type == "Player") {
		SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 272.0f / 2048.0f, 0.0f / 2048.0f, 132.0f / 2048.0f, 165.0f / 2048.0f);
		player = new Player();
		player->sprite = playerSprite;
		player->x = placeX;
		player->y = placeY;
		player->setScale(3.0f);
		player->friction_x = 3.0f;
		player->hp = 5;
		entities.push_back(player);
	}
}

void SideScroller::RenderLevel() {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, brickSpriteSheetTexture);

	vector<float> vertexData;
	vector<float> texCoordData;

	int numVertices = 0;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0) {
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				float spacing = 1.0f / 924.0f;

				vertexData.insert(vertexData.end(), {
					TILE_SIZE* x, -TILE_SIZE* y,
					TILE_SIZE* x, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, -TILE_SIZE* y
				});
				texCoordData.insert(texCoordData.end(), { u + spacing, v + spacing,
					u + spacing, v + (spriteHeight)-spacing,
					u + spriteWidth - spacing, v + (spriteHeight)-spacing,
					u + spriteWidth - spacing, v + spacing
				});

				numVertices += 4;
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	Matrix tempMatrix;
	tempMatrix.identity();
	tempMatrix.m[3][0] = -TILE_SIZE* mapWidth / 2;
	tempMatrix.m[3][1] = TILE_SIZE* mapHeight / 2, 0.0f;
	glMultMatrixf(tempMatrix.ml);

	glDrawArrays(GL_QUADS, 0, numVertices);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

bool SideScroller::isSolid(unsigned char tile) {
	switch (tile) {
	case 0:
		return false;
		break;
	default:
		return true;
		break;
	}
}

void SideScroller::worldToTileCoordinates(float worldX, float worldY, int*gridX, int*gridY) {
	*gridX = (int)(((worldX * 16.0f) / TILE_SIZE) + (mapWidth * 16.0f / 2.0f)) / 16.0f;
	*gridY = (int)(((worldY * 16.0f) / -TILE_SIZE) + (mapHeight * 16.0f / 2.0f)) / 16.0f;
}

float SideScroller::checkPointForGridCollisionX(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight) {
		return 0.0f;
	}
		
	if (isSolid(levelData[gridY][gridX])) {
		float xCoordinate = (((gridX * 16.0f - (mapWidth * 16.0f / 2.0f)) * TILE_SIZE) / 16.0f) + TILE_SIZE / 2.0f;
		return fabs(x - xCoordinate) - (TILE_SIZE / 2.0f);
	}
	return 0.0f;
}

float SideScroller::checkPointForGridCollisionY(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight) {
		return 0.0f;
	}

	if (isSolid(levelData[gridY][gridX])) {
		float yCoordinate = (((gridY * 16.0f - (mapHeight * 16.0f / 2.0f)) * -TILE_SIZE) / 16.0f) + TILE_SIZE / 2.0f;
		return fabs(y - yCoordinate) - (TILE_SIZE / 2.0f);
		
	}
	return 0.0f;
}

void SideScroller::doLevelCollisionX(Entity *entity) {
	//check right

	float adjust = checkPointForGridCollisionX(entity->x + entity->width*0.5, entity->y);
	if (adjust == 0.0f) {
		adjust = checkPointForGridCollisionX(entity->x + entity->width*0.5, entity->y + entity->height*0.5);
	}
	if (adjust != 0.0f) {
		entity->x += adjust;
		entity->velocity_x = 0.0f;
		entity->collidedRight = true;
	}

	//check left

	adjust = checkPointForGridCollisionX(entity->x - entity->width*0.5, entity->y);
	if (adjust == 0.0f) {
		adjust = checkPointForGridCollisionX(entity->x - entity->width*0.5, entity->y + entity->height*0.5);
	}
	if (adjust != 0.0f) {
		entity->x -= adjust;
		entity->velocity_x = 0.0f;
		entity->collidedLeft = true;
	}


}

void SideScroller::doLevelCollisionY(Entity *entity) {
	//check bottom

	float adjust = checkPointForGridCollisionY(entity->x, entity->y - entity->height*0.5);
	if (adjust != 0.0f) {
		entity->y += adjust;
		entity->velocity_y = 0.0f;
		entity->collidedBottom = true;
	}

	//check top

	adjust = checkPointForGridCollisionY(entity->x, entity->y + entity->height*0.5);
	if (adjust != 0.0f) {
		entity->y -= adjust;
		entity->velocity_y = 0.0f;
		entity->collidedTop = true;
	}
}

void DrawText(int textureID, string text, float size, float spacing, float r, float g, float b, float a)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<float> vertexData;
	vector<float> uvData;
	vector<float> colorData;

	float textureSize = 1.0f / 16.0f;

	for (int i = 0; i < text.size(); i++)
	{
		float textureX = (float)((int)text[i] % 16) / 16.0f;
		float textureY = (float)((int)text[i] / 16) / 16.0f;

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
			* size });
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		uvData.insert(uvData.end(), { textureX, textureY, textureX, textureY + textureSize, textureX + textureSize, textureY + textureSize, textureX + textureSize, textureY });
	}
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, uvData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}