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

	brickSpriteSheetTexture = LoadTexture("resources/spriteTiles.png");
	characterSpriteSheetTexture = LoadTexture("resources/Sprites_Characters.png");
	characterAnimationSpriteSheetTexture = LoadTexture("resources/character_animations.png");
	fontTexture = LoadTexture("resources/pixel_font.png");
	weaponSpriteSheetTexture = LoadTexture("resources/sheet.png");

	projectile_raygun_bullet_sprite = SheetSprite(weaponSpriteSheetTexture, 14, 7, 7);

	//Weapon sprites
	weapon_raygun_sprite = SheetSprite(weaponSpriteSheetTexture, 14, 7, 2);
	
	//Player static sprites
	player_sprite = SheetSprite(characterSpriteSheetTexture, 272.0f / 2048.0f, 0.0f / 2048.0f, 132.0f / 2048.0f, 165.0f / 2048.0f);
	player_sprite_face_right = SheetSprite(characterSpriteSheetTexture, 918.0f / 2048.0f, 1323.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f);
	player_sprite_face_left = SheetSprite(characterSpriteSheetTexture, 1039.0f / 2048.0f, 1490.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f);
	player_sprite_jump_right = SheetSprite(characterAnimationSpriteSheetTexture, 840.0f / 1320.0f, 1137.0f / 1320.0f, 120.0f / 1320.0f, 165.0f / 1320.0f);
	player_sprite_jump_left = SheetSprite(characterAnimationSpriteSheetTexture, 243.0f / 1320.0f, 165.0f / 1320.0f, 120.0f / 1320.0f, 165.0f / 1320.0f);

	//Player animated sprites
	player_frames_walk_right.push_back({ 918.0f / 2048.0f, 1323.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	player_frames_walk_right.push_back({ 918.0f / 2048.0f, 662.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	player_frames_walk_right.push_back({ 917.0f / 2048.0f, 495.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	player_frames_walk_left.push_back({ 1039.0f / 2048.0f, 1490.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	player_frames_walk_left.push_back({ 1038.0f / 2048.0f, 996.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	player_frames_walk_left.push_back({ 1038.0f / 2048.0f, 829.0f / 2048.0f, 120.0f / 2048.0f, 165.0f / 2048.0f });
	
	//Enemy static sprites
	enemy_sprite = SheetSprite(characterSpriteSheetTexture, 405.0f / 2048.0f, 167.0f / 2048.0f, 132.0f / 2048.0f, 161.0f / 2048.0f);
	enemy_sprite_face_right = SheetSprite(characterSpriteSheetTexture, 1160.0f / 2048.0f, 827.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f);
	enemy_sprite_face_left = SheetSprite(characterSpriteSheetTexture, 916.0f / 2048.0f, 995.0f / 2048.0f, 120.0f / 2048.0f, 161.0f / 2048.0f);
	enemy_sprite_jump_right = SheetSprite(characterAnimationSpriteSheetTexture, 122.0f / 1320.0f, 476.0f / 1320.0f, 120.0f / 1320.0f, 161.0f / 1320.0f);
	enemy_sprite_jump_left = SheetSprite(characterAnimationSpriteSheetTexture, 721.0f / 1320.0f, 483.0f / 1320.0f, 120.0f / 1320.0f, 161.0f / 1320.0f);

	//Enemy animated sprites
	enemy_frames_walk_right.push_back({ 1160.0f / 2048.0f, 827.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	enemy_frames_walk_right.push_back({ 1158.0f / 2048.0f, 0.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	enemy_frames_walk_right.push_back({ 795.0f / 2048.0f, 1820.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	enemy_frames_walk_left.push_back({ 916.0f / 2048.0f, 995.0f / 2048.0f, 120.0f / 2048.0f, 161.0f / 2048.0f });
	enemy_frames_walk_left.push_back({ 1040.0f / 2048.0f, 1163.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	enemy_frames_walk_left.push_back({ 796.0f / 2048.0f, 831.0f / 2048.0f, 120.0f / 2048.0f, 162.0f / 2048.0f });
	 
	gunshot = Mix_LoadWAV("resources/gunshot.wav");
	jump = Mix_LoadWAV("resources/jump.wav");
	//music = Mix_LoadMUS("resources/music.wav");
	if (Mix_PlayMusic(music, -1) < 0) {
		cout << "Error";
	}
	
}

SideScroller::~SideScroller() {
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

}

void SideScroller::Update(float elapsed) {
	if (state == STATE_TITLE)
	{
		return;
	}
	else if (state == STATE_GAME)
	{
		if (enemySpawnTimer > 0.5f && enemies.size() < 50) {
			Enemy* tempEnemy = new PewRunner();
			tempEnemy->sprite = enemy_sprite;
			tempEnemy->y = player->y + 4.0f;
			tempEnemy->x = player->x + 10.0f;
			tempEnemy->setScale(2.5f);
			tempEnemy->setWalkLeft(0.8f);

			tempEnemy->sprite_face_right = enemy_sprite_face_right;
			tempEnemy->sprite_face_left = enemy_sprite_face_left;
			
			tempEnemy->sprite_jump_right = enemy_sprite_jump_right;
			tempEnemy->sprite_jump_left = enemy_sprite_jump_left;

			//walk right animation
			tempEnemy->animation_walk_right = SheetSprite(characterSpriteSheetTexture);
			tempEnemy->animation_walk_right.setAnimated(true, 8.0f, enemy_frames_walk_right);

			//walk left animation
			tempEnemy->animation_walk_left = SheetSprite(characterSpriteSheetTexture);
			tempEnemy->animation_walk_left.setAnimated(true, 8.0f, enemy_frames_walk_left);


			enemies.push_back(tempEnemy);
			entities.push_back(tempEnemy);
			
			enemySpawnTimer = 0.0f;
		}

		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->Update(elapsed);
		}

		for (size_t i = 0; i < projectiles.size(); i++) {
			projectiles[i]->Update(elapsed);
		}

		enemySpawnTimer += elapsed;
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
			
			if (entities[i]->gravity_affected) {
				entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
				entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
			}
			
			if (entities[i]->velocity_y <= -5.5f) {
				entities[i]->velocity_y = -5.5f;
			}
			
			entities[i]->FixedUpdate();

			entities[i]->y += entities[i]->velocity_y * FIXED_TIMESTEP;
			//do Y collisions
			if (!entities[i]->isStatic) {
				for (size_t j = 0; j < entities.size(); j++) {

					
					if (Player* player = dynamic_cast<Player*>(entities[i])) {
						if (Weapon* weapon = dynamic_cast<Weapon*>(entities[j])) {
							if (player->collidesWith(weapon) && weapon->gravity_affected) {
								player->equip(weapon);
							}
						}
					}

					if (entities[i]->collidesWith(entities[j]) && entities[i] != entities[j] && !entities[j]->isStatic) {
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
					if (entities[i]->collidesWith(entities[j]) && entities[i] != entities[j] && !entities[j]->isStatic) {
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

		for each (auto enemy in enemies) {
			if (sqrt(pow(enemy->x - player->x, 2) + pow(enemy->y - player->y, 2)) < 3.0f) {
				enemy->near_player = true;
			}
			else
				enemy->near_player = false;
	

			//shoot ai
			if (enemy->aiShootTimer > 1.5f && enemy->near_player) {
				Projectile* tempProjectile = new Projectile();
				tempProjectile->sprite = projectile_raygun_bullet_sprite;
				tempProjectile->changeProjectile(7.5f, 7.5f, 2);
				projectiles.push_back(tempProjectile);
				enemy->shoot(tempProjectile);
				enemy->aiShootTimer = 0.0f;
			}
			
			

			//jump ai
			float point_1F_x = 0.0f;
			float point_1F_y = 0.0f;
			float point_1B_x = 0.0f;
			float point_1B_y = 0.0f;
			float point_1F2T_x = 0.0f;
			float point_1F2T_y = 0.0f;
			float point_3F2T_x = 0.0f;
			float point_3F2T_y = 0.0f;
			float point_1F1B_x = 0.0f;
			float point_1F1B_y = 0.0f;

			if (enemy->face_left) {
				point_1F_x = enemy->x - 0.2f;
				point_1B_x = enemy->x;
				point_1F2T_x = enemy->x - 0.2f;
				point_3F2T_x = enemy->x - 0.6f;
				point_1F1B_x = enemy->x - 0.2f;
			}
				
			else {
				point_1F_x = enemy->x + 0.2f;
				point_1B_x = enemy->x;
				point_1F2T_x = enemy->x + 0.2f;
				point_3F2T_x = enemy->x + 0.6f;
				point_1F1B_x = enemy->x + 0.2f;
			}
			
			point_1F_y = enemy->y;
			point_1B_y = enemy->y - 0.2f;
			point_1F2T_y = enemy->y + 0.4f;
			point_3F2T_y = enemy->y + 0.4f;
			point_1F1B_y = enemy->y - 0.2f;

			
			if (enemy->is_jumper) {
				//jump up to another platform
				if (checkPointForGridCollisionX(point_3F2T_x, point_3F2T_y) != 0) {
					if (checkPointForGridCollisionX(point_1F2T_x, point_1F2T_y) == 0)
						enemy->jump();
				}

				//jump pass obstacle
				if (checkPointForGridCollisionX(point_1F_x, point_1F_y) != 0) {
					if (checkPointForGridCollisionX(point_1F2T_x, point_1F2T_y) == 0)
						enemy->jump();
				}
			}
		
			//go backwards near dead-end cliff
			if (!enemy->is_jumper) {
				if (checkPointForGridCollisionX(point_1F1B_x, point_1F1B_y) == 0) {
					if (checkPointForGridCollisionX(point_1B_x, point_1B_y) != 0)
						if (enemy->face_left)
							enemy->setWalkRight(1.0f);
						else
							enemy->setWalkLeft(1.0f);
				}
			}
			

			//enemy gets hit
			for each (auto projectile in projectiles) {
				if (checkPointForGridCollisionX(projectile->x, projectile->y) != 0)
					projectile->should_remove = true;
				if (enemy->collidesWith(projectile) && !projectile->should_remove) {
					projectile->should_remove = true;
					enemy->hp -= projectile->damage;
				}
			}
		}


		//clean up memory caused by entities
		for each (auto entity in entities) {
			if (entity->should_remove) {
				entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
				enemies.erase(std::remove(enemies.begin(), enemies.end(), entity), enemies.end());
				delete entity;
				break;
			}

		}

		//clean up memory caused by projectiles
		for each (auto projectile in projectiles) {
			if (projectile->should_remove) {
				projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), projectile), projectiles.end());
				delete projectile;
				break;
			}

		}

		//check if player is dead
		if (player->hp <= 0)
			state = STATE_GAMEOVER;
	}
}

void SideScroller::Render(float elapsed) {
	//glClearColor(173.0f/256.0f, 216.0f/256.0f, 230.0f/256.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	if (state == STATE_TITLE)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Matrix tempMatrix;
		tempMatrix.identity();
		tempMatrix.m[3][0] = -1.2f;
		tempMatrix.m[3][1] = -0.2f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(fontTexture, "Press 1 for Level Green", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		tempMatrix.identity();
		tempMatrix.m[3][1] = -0.2f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(fontTexture, "Press 2 for Level Blue", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		tempMatrix.identity();
		tempMatrix.m[3][1] = -0.2f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(fontTexture, "Press 3 for Level Pink", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		tempMatrix.identity();
		tempMatrix.m[3][1] = -0.2f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(fontTexture, "Press Q to quit", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
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
		
		if (player->weapon) {
			tempMatrix.identity();
			tempMatrix.m[3][0] = 1.4f;
			glMultMatrixf(tempMatrix.ml);
			DrawText(fontTexture, "AMMO: " + to_string(player->weapon->ammo) + "/" + to_string(player->weapon->max_ammo), 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		glPopMatrix();

		float translateX = -player->x;
		float translateY = -player->y;

		//scrolling boundaries
		if (translateY > (float)(mapHeight * TILE_SIZE / 2.0f) - 2.0f - 0.2f)
			translateY = (float)(mapHeight * TILE_SIZE / 2.0f) - 2.0f - 0.2f;
		else if (translateY < -(float)(mapHeight * TILE_SIZE / 2.0f) + 2.00f)
			translateY = -(float)(mapHeight * TILE_SIZE / 2.0f) + 2.00f;

		if (translateX >(float)(mapWidth * TILE_SIZE / 2.0f) - 2.66f)
			translateX = (float)(mapWidth * TILE_SIZE / 2.0f) - 2.66f;
		else if (translateX < (float)(-1.0f * mapWidth * TILE_SIZE / 2.0f) + 2.66f)
			translateX = (float)(-1.0f * mapWidth * TILE_SIZE / 2.0f) + 2.66f;

		//check if player reached the end
		if (player->x >= 19.1)
			state = STATE_GAMEOVER;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		tempMatrix.identity();
		tempMatrix.m[3][0] = translateX;
		tempMatrix.m[3][1] = translateY;
		glMultMatrixf(tempMatrix.ml);
		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->Render(elapsed);
		}

		for (size_t i = 0; i < projectiles.size(); i++) {
			projectiles[i]->Render(elapsed);
		}
		RenderLevel();
	}
	else if (state = STATE_GAMEOVER)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Matrix tempMatrix;
		tempMatrix.identity();
		if (player->hp <= 0)
		{
			tempMatrix.m[3][0] = -0.8f;
			glMultMatrixf(tempMatrix.ml);
			DrawText(fontTexture, "Game Over! You Died!", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			tempMatrix.identity();
			tempMatrix.m[3][0] = -0.8f;
			tempMatrix.m[3][1] = -0.8f;
			glMultMatrixf(tempMatrix.ml);
		}
		else
		{
			tempMatrix.m[3][0] = -0.5f;
			glMultMatrixf(tempMatrix.ml);
			DrawText(fontTexture, "You Won!", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			tempMatrix.identity();
			tempMatrix.m[3][0] = -1.1f;
			tempMatrix.m[3][1] = -0.8f;
			glMultMatrixf(tempMatrix.ml);
		}
		DrawText(fontTexture, "Press SPACE to return to main menu", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
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
				if (state == STATE_GAMEOVER)
				{
					state = STATE_TITLE;
					resetGame();
				}
			}
			else if (state == STATE_TITLE && event.key.keysym.scancode == SDL_SCANCODE_1)
			{
				buildLevel("resources/Level1.txt");
				state = STATE_GAME;
			}
			else if (state == STATE_TITLE && event.key.keysym.scancode == SDL_SCANCODE_2)
			{
				buildLevel("resources/Level2.txt");
				state = STATE_GAME;
			}
			else if (state == STATE_TITLE && event.key.keysym.scancode == SDL_SCANCODE_Q)
			{
				done = true;
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
			if (player->jump()) {
				Mix_PlayChannel(-1, jump, 0);
			}
		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			player->setWalkRight(1.0f);
		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			player->setWalkLeft(1.0f);
		}
		else {
			player->setIdle();
		}
		if (keys[SDL_SCANCODE_SPACE]) {


			Projectile* tempProjectile = new Projectile();
			tempProjectile->sprite = projectile_raygun_bullet_sprite;
			tempProjectile->changeProjectile(7.5f, 7.5f, 2);
			projectiles.push_back(tempProjectile);
			if (player->shoot(tempProjectile)) {
				Mix_PlayChannel(-1, gunshot, 0);
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
	Render(elapsed);
	return done;
}

void SideScroller::resetGame()
{
	//clean up memory caused by entities
	for each (auto entity in entities) {
		delete entity;
	}

	//clean up memory caused by projectiles
	for each (auto projectile in projectiles) {
		delete projectile;
	}

	entities.clear();
	enemies.clear();
	projectiles.clear();

}

void SideScroller::buildLevel(string filename) {
	ifstream infile(filename);
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
		player = new Player();
		player->sprite = player_sprite;
		player->x = placeX;
		player->y = placeY;
		player->setScale(3.0f);
		player->friction_x = 3.0f;
		player->hp = 5;

		player->sprite_face_right = player_sprite_face_right;
		player->sprite_face_left = player_sprite_face_left;
		player->sprite_jump_right = player_sprite_jump_right;
		player->sprite_jump_left = player_sprite_jump_left;

		//walk right animation
		player->animation_walk_right = SheetSprite(characterSpriteSheetTexture);
		player->animation_walk_right.setAnimated(true, 8.0f, player_frames_walk_right);
		

		//walk left animation
		player->animation_walk_left = SheetSprite(characterSpriteSheetTexture);
		player->animation_walk_left.setAnimated(true, 8.0f, player_frames_walk_left);

		Weapon* weapon = new Weapon();
		weapon->sprite = weapon_raygun_sprite;
		weapon->changeWeapon(RAY_GUN, 1.5f, 1.5f, 1.0f, 150, 350);
		entities.push_back(weapon);
		player->equip(weapon);

		entities.push_back(player);
	} 
	else if (type == "PewRunner") {
		Enemy* enemy = new PewRunner();
		enemy->sprite = enemy_sprite;
		enemy->x = placeX;
		enemy->y = placeY;
		enemy->setScale(2.5f);
		enemy->setWalkLeft(0.8f);

		enemy->sprite_face_right = enemy_sprite_face_right;
		enemy->sprite_face_left = enemy_sprite_face_left;
		enemy->sprite_jump_right = enemy_sprite_jump_right;
		enemy->sprite_jump_left = enemy_sprite_jump_left;

		//walk right animation
		enemy->animation_walk_right = SheetSprite(characterSpriteSheetTexture);
		enemy->animation_walk_right.setAnimated(true, 8.0f, enemy_frames_walk_right);


		//walk left animation
		enemy->animation_walk_left = SheetSprite(characterSpriteSheetTexture);
		enemy->animation_walk_left.setAnimated(true, 8.0f, enemy_frames_walk_left);

		enemies.push_back(enemy);
		entities.push_back(enemy);
	}
	else if (type == "PewShooter") {
		Enemy* enemy = new PewShooter();
		enemy->sprite = enemy_sprite;
		enemy->x = placeX;
		enemy->y = placeY;
		enemy->setScale(2.5f);
		enemy->setWalkLeft(0.8f);

		enemy->sprite_face_right = enemy_sprite_face_right;
		enemy->sprite_face_left = enemy_sprite_face_left;
		enemy->sprite_jump_right = enemy_sprite_jump_right;
		enemy->sprite_jump_left = enemy_sprite_jump_left;

		//walk right animation
		enemy->animation_walk_right = SheetSprite(characterSpriteSheetTexture);
		enemy->animation_walk_right.setAnimated(true, 8.0f, enemy_frames_walk_right);


		//walk left animation
		enemy->animation_walk_left = SheetSprite(characterSpriteSheetTexture);
		enemy->animation_walk_left.setAnimated(true, 8.0f, enemy_frames_walk_left);

		Weapon* weapon = new Weapon();
		weapon->sprite = weapon_raygun_sprite;
		weapon->sprite.invert = true;
		weapon->changeWeapon(RAY_GUN, 1.5f, 1.5f, 1.0f, 150, 350);
		enemy->equip(weapon);
		entities.push_back(weapon);
		
		enemies.push_back(enemy);
		entities.push_back(enemy);

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
	case 189:
		return false;
		break;
	case 187:
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