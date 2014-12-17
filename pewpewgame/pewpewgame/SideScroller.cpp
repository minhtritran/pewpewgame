#include "SideScroller.h"

enum GameState { STATE_TITLE, STATE_GAME, STATE_GAMEOVER };

SideScroller::SideScroller() {
	Init();
	srand(time(NULL));

	state = STATE_TITLE;
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	gravity_x = 0.0f;
	gravity_y = -9.8f;

	tex = new Textures();
	 
	gunshot = Mix_LoadWAV("resources/gunshot.wav");
	lasershot = Mix_LoadWAV("resources/lasershot.wav");
	smileyshot = Mix_LoadWAV("resources/smileyshot.wav");
	fruitshot = Mix_LoadWAV("resources/fruitshot.wav");
	jump = Mix_LoadWAV("resources/jump.wav");
	hurt = Mix_LoadWAV("resources/hurt.wav");
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
		if (enemySpawnTimer > 0.55f && enemies.size() < 50) {
			Enemy* tempEnemy = new PewRunner(tex);
			tempEnemy->y = player->y + 4.0f;
			tempEnemy->x = player->x + 10.0f;
			tempEnemy->setScale(2.5f);

			if (genRandomNumber(0.0f, 1.0f) > 0.93f) {
				//Grant him knighthood
				Weapon* weapon = new Weapon(tex);
				weapon->changeWeapon(SWORD);
				tempEnemy->equip(weapon);
				entities.push_back(weapon);
			}

			tempEnemy->setWalkLeft(0.8f);

			enemies.push_back(tempEnemy);
			entities.push_back(tempEnemy);
			
			enemySpawnTimer = genRandomNumber(0.0f, 0.5f);
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
			enemy->dist_to_player = sqrt(pow(enemy->x - player->x, 2) + pow(enemy->y - player->y, 2));

			//shoot ai
			if (enemy->aiShootTimer > 1.5f && enemy->dist_to_player < 3.0f) {
				Projectile* tempProjectile = enemy->shoot();
				if (tempProjectile != NULL)  {
					tempProjectile->damages_player = true;
					projectiles.push_back(tempProjectile);
					enemy->aiShootTimer = 0.0f;
				}
			}
			
			//melee ai
			if (enemy->aiMeleeTimer > 0.2f && enemy->dist_to_player < 0.2f) {
				int damage = enemy->melee();
				if (damage != 0) {
					if ((player->face_left && enemy->face_left && enemy->x < player->x) || (!player->face_left && !enemy->face_left && enemy->x > player->x)) {

					}
					else
					{
						Mix_PlayChannel(-1, hurt, 0);
						player->hp -= damage;
					}
				}
				enemy->aiMeleeTimer = 0.0f;
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

				//jump pass pews
				for each (auto otherenemy in enemies) {
					//the addtional 0.1f helps resolve pew jumping deadlocks
					if (point_1F_x > otherenemy->x - otherenemy->width + 0.1f && point_1F_x < otherenemy->x + otherenemy->width + 0.1f)
						if (point_1F_y > otherenemy->y - otherenemy->height && point_1F_y < otherenemy->y + otherenemy->height)
							enemy->jump();
				}

				//if jumper is behind and far away from player, move him forward 
				if (enemy->dist_to_player > 10.0f && enemy->x < player->x) {
					enemy->x = player->x + 10.0f;
					enemy->y = player->y + 4.0f;
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
		}

		//stuff gets hit
		for each (auto projectile in projectiles) {
			if (projectile->damages_enemy) {
				for each (auto enemy in enemies) {
					if (checkPointForGridCollisionX(projectile->x, projectile->y) != 0)
						projectile->should_remove = true;
					if (enemy->collidesWith(projectile) && !projectile->should_remove) {
						projectile->should_remove = true;
						enemy->hp -= projectile->damage;
						enemy->velocity_y += 0.5f;
						if (projectile->velocity_x > 0)
							enemy->velocity_x += 0.5f;
						else
							enemy->velocity_x -= 0.5f;
					}
				}
			}
			if (projectile->damages_player) {
				if (checkPointForGridCollisionX(projectile->x, projectile->y) != 0)
					projectile->should_remove = true;
				if (player->collidesWith(projectile) && !projectile->should_remove) {
					projectile->should_remove = true;
					Mix_PlayChannel(-1, hurt, 0);
					player->hp -= projectile->damage;
					if (projectile->velocity_x > 0)
						player->velocity_x += 2.5f;
					else
						player->velocity_x -= 2.5f;
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
	glClearColor(25.0f/256.0f, 72.0f/256.0f, 148.0f/256.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	if (state == STATE_TITLE)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Matrix tempMatrix;
		tempMatrix.identity();
		tempMatrix.m[3][0] = -0.9f;
		tempMatrix.m[3][1] = 0.9f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(tex->fontTexture, "Pew Pew", 0.3f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		tempMatrix.identity();
		tempMatrix.m[3][0] = -0.2f;
		tempMatrix.m[3][1] = -1.3f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(tex->fontTexture, "Press 1 for Level Green", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		tempMatrix.identity();
		tempMatrix.m[3][1] = -0.2f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(tex->fontTexture, "Press 2 for Level Blue", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		tempMatrix.identity();
		tempMatrix.m[3][1] = -0.2f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(tex->fontTexture, "Press 3 for Level Pink", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		tempMatrix.identity();
		tempMatrix.m[3][1] = -0.2f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(tex->fontTexture, "Press Q to quit", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (state == STATE_GAME)
	{
		Matrix tempMatrix;

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

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		tempMatrix.identity();
		tempMatrix.m[3][0] = -2.4f;
		tempMatrix.m[3][1] = 1.85f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(tex->fontTexture, "HP: " + to_string(player->hp) + "/100", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		if (player->weapon) {
			tempMatrix.identity();
			tempMatrix.m[3][0] = 1.4f;
			glMultMatrixf(tempMatrix.ml);
			DrawText(tex->fontTexture, "AMMO: " + to_string(player->weapon->ammo) + "/" + to_string(player->weapon->max_ammo), 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		glPopMatrix();
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
			DrawText(tex->fontTexture, "Game Over! You Died!", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			tempMatrix.identity();
			tempMatrix.m[3][0] = -0.8f;
			tempMatrix.m[3][1] = -0.8f;
			glMultMatrixf(tempMatrix.ml);
		}
		else
		{
			tempMatrix.m[3][0] = -0.5f;
			glMultMatrixf(tempMatrix.ml);
			DrawText(tex->fontTexture, "You Won!", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			tempMatrix.identity();
			tempMatrix.m[3][0] = -1.1f;
			tempMatrix.m[3][1] = -0.8f;
			glMultMatrixf(tempMatrix.ml);
		}
		DrawText(tex->fontTexture, "Press ENTER to return to main menu", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
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
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER) {
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
			else if (state == STATE_TITLE && event.key.keysym.scancode == SDL_SCANCODE_3)
			{
				buildLevel("resources/Level3.txt");
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
			Projectile* tempProjectile = player->shoot();
			if (tempProjectile != NULL)  {
				tempProjectile->damages_enemy = true;
				projectiles.push_back(tempProjectile);
				if (player->weapon->bullet_type == PLASMA_BALL)
					Mix_PlayChannel(-1, gunshot, 0);
				else if (player->weapon->bullet_type == LASER)
					Mix_PlayChannel(-1, lasershot, 0);
				else if (player->weapon->bullet_type == SMILEY_FACE)
					Mix_PlayChannel(-1, smileyshot, 0);
				else if (player->weapon->bullet_type == FRUIT)
					Mix_PlayChannel(-1, fruitshot, 0);
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
		player = new Player(tex);
		player->x = placeX;
		player->y = placeY;
		player->setScale(3.0f);
		player->friction_x = 3.0f;
		player->hp = 100;

		Weapon* weapon = new Weapon(tex);
		//weapon->changeWeapon(RAY_GUN);
		//weapon->changeWeapon(MINI_GUN);
		//weapon->changeWeapon(MACHINE_GUN); 
		weapon->changeWeapon(SHOTGUN);
		player->equip(weapon);
		entities.push_back(weapon);


		entities.push_back(player);
	} 
	else if (type == "PewRunner") {
		Enemy* enemy = new PewRunner(tex);
		enemy->x = placeX;
		enemy->y = placeY;
		enemy->setScale(2.5f);

		Weapon* weapon = new Weapon(tex);
		weapon->changeWeapon(SWORD);
		enemy->equip(weapon);
		entities.push_back(weapon);

		enemy->setWalkLeft(0.8f);

		enemies.push_back(enemy);
		entities.push_back(enemy);
	}
	else if (type == "PewShooter") {
		Enemy* enemy = new PewShooter(tex);
		enemy->x = placeX;
		enemy->y = placeY;
		enemy->setScale(2.5f);
		
		Weapon* weapon = new Weapon(tex);
		weapon->changeWeapon(RAY_GUN);
		enemy->equip(weapon);
		entities.push_back(weapon);
		

		enemy->setWalkLeft(0.8f);

		enemies.push_back(enemy);
		entities.push_back(enemy);

	}
}

void SideScroller::RenderLevel() {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->brickSpriteSheetTexture);

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



float genRandomNumber(float low, float high) {
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}