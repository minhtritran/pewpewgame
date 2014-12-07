#include "SideScroller.h"

SideScroller::SideScroller() {
	Init();
	

	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	gravity_x = 0.0f;
	gravity_y = -9.8f;

	bulletIndex = 0;
	shootTimer = 0.1f;

	brickSpriteSheetTexture = LoadTexture("sheet_4.png");
	characterSpriteSheetTexture = LoadTexture("characters_1.png");
	bulletSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 3);
	buildLevel();

	gunshot = Mix_LoadWAV("gunshot.wav");
	jump = Mix_LoadWAV("jump.wav");
	music = Mix_LoadMUS("music.wav");
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
	if (enemySpawnTimer > 1.0f && enemyIndex < 8) {
		SheetSprite enemySprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 2);
		enemies[enemyIndex].sprite = enemySprite;
		enemies[enemyIndex].y = 0.85f;
		enemies[enemyIndex].x = 0.65f;
		enemies[enemyIndex].width = 0.2f;
		enemies[enemyIndex].height = 0.2f;
		enemies[enemyIndex].acceleration_x = -2.0f;
		entities.push_back(&enemies[enemyIndex]);
		enemyIndex++;
		enemySpawnTimer = 0.0f;
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}

	shootTimer += elapsed;
	enemySpawnTimer += elapsed;
}

void SideScroller::FixedUpdate() {

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate();

		if (entities[i]->collidedBottom) {
			entities[i]->isJumping = false;
			entities[i]->velocity_y = 0.0f;
		}
		if (entities[i]->collidedTop)
			entities[i]->velocity_y = 0.0f;
		if (entities[i]->collidedLeft)
			entities[i]->velocity_x = 0.0f;
		if (entities[i]->collidedRight)
			entities[i]->velocity_x = 0.0f;

		entities[i]->collidedBottom = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;

		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}

		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

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
		if (enemies[i].collidedRight) {
			enemies[i].acceleration_x = -2.0f;
		}

		if (enemies[i].collidedLeft) {
			enemies[i].acceleration_x = 2.0f;
		}
		//enemy gets hit
		for (int k = 0; k < MAX_BULLETS; k++) {
			if (enemies[i].collidesWith(&bullets[k])) {
				bullets[k].visible = false;
				enemies[i].y = 1.3f;
				enemies[i].x = 0.0f;
			}
		}
			
	}
}

void SideScroller::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	float translateX = -player->x;
	float translateY = -player->y;

	if (translateY > 1.5)
		translateY = 1.5;
	if (translateX > 8.0)
		translateX = 8.0;
	if (translateX < -8.0)
		translateX = -8.0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(translateX, translateY, 0.0f);



	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}
	RenderLevel();

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
				if (shootTimer > 0.1f)
					shootBullet();
			}
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W]) {
		if (!player->isJumping) {
			Mix_PlayChannel(-1, jump, 0);
			player->jump();
		}
		
	}
	if (keys[SDL_SCANCODE_D]) {
		SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 27);
		player->sprite = playerSprite;
		player->setWalkRight();
	}
	else if (keys[SDL_SCANCODE_A]) {
		SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 15);
		player->sprite = playerSprite;
		player->setWalkLeft();
	}
	else {
		player->setIdle();
	}
	if (keys[SDL_SCANCODE_X]) {
		if (shootTimer > 0.1f) {
			shootTimer = 0.0f;
			shootBullet();
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
	ifstream infile("levelFile.txt");
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
						levelData[y][x] = val;
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
			
			float placeX = (atoi(xPosition.c_str()) + 8.0f - 128.0f * 16.0f / 2.0f) / 16.0f * TILE_SIZE;
			float placeY = (atoi(yPosition.c_str()) - 8.0f - 32.0f * 16.0f / 2.0f) / 16.0f * -TILE_SIZE;
			
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void SideScroller::placeEntity(string& type, float placeX, float placeY) {
	if (type == "Player") {
		SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 15);
		player = new Player();
		player->sprite = playerSprite;
		player->x = placeX;
		player->y = placeY;
		player->width = 0.2f;
		player->height = 0.2f;
		player->friction_x = 3.0f;
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
				vertexData.insert(vertexData.end(), {
					TILE_SIZE* x, -TILE_SIZE* y,
					TILE_SIZE* x, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, -TILE_SIZE* y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
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
	glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);

	glDrawArrays(GL_QUADS, 0, numVertices);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

void SideScroller::shootBullet() {
	Mix_PlayChannel(-1, gunshot, 0);
	bullets[bulletIndex].sprite = bulletSprite;
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = player->x;
	bullets[bulletIndex].y = player->y;
	bullets[bulletIndex].rotation = 0.0f;
	if (player->face_left)
		bullets[bulletIndex].velocity_x = -3.5f;
	else
		bullets[bulletIndex].velocity_x = 3.5f;
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}
	shootTimer = 0;
}

bool SideScroller::isSolid(unsigned char tile) {
	switch (tile) {
	case 9:
		return true;
		break;
	case 11:
		return true;
		break;
	default:
		return false;
		break;
	}
}

void SideScroller::worldToTileCoordinates(float worldX, float worldY, int*gridX, int*gridY) {
	*gridX = (int)(((worldX * 16.0f) / TILE_SIZE) + (128.0f * 16.0f / 2.0f)) / 16.0f;
	*gridY = (int)(((worldY * 16.0f) / -TILE_SIZE) + (32.0f * 16.0f / 2.0f)) / 16.0f;
}

float SideScroller::checkPointForGridCollisionX(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 128 || gridY < 0 || gridY > 32) {
		return 0.0f;
	}
		
	if (isSolid(levelData[gridY][gridX])) {
		float xCoordinate = (((gridX * 16.0f - (128.0f * 16.0f / 2.0f)) * TILE_SIZE) / 16.0f) + TILE_SIZE/2.0f;
		return fabs(x - xCoordinate) - (TILE_SIZE / 2.0f);
	}
	return 0.0f;
}

float SideScroller::checkPointForGridCollisionY(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 128 || gridY < 0 || gridY > 32) {
		return 0.0f;
	}

	if (isSolid(levelData[gridY][gridX])) {
		float yCoordinate = (((gridY * 16.0f - (32.0f * 16.0f / 2.0f)) * -TILE_SIZE) / 16.0f) + TILE_SIZE / 2.0f;
		return fabs(y - yCoordinate) - (TILE_SIZE / 2.0f);
		
	}
	return 0.0f;
}

void SideScroller::doLevelCollisionX(Entity *entity) {
	//check right

	float adjust = checkPointForGridCollisionX(entity->x + entity->width*0.5, entity->y);
	if (adjust != 0.0f) {
		entity->x += adjust;
		entity->velocity_x = 0.0f;
		entity->collidedRight = true;
	}

	//check left

	adjust = checkPointForGridCollisionX(entity->x - entity->width*0.5, entity->y);
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
		entity->velocity_y += 0.0f;
		entity->collidedTop = true;
	}
}

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}