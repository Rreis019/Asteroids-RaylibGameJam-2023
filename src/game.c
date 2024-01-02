#include "game.h"
#include "Card.h"
#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_types.h"
#include "controller.h"
#include "enemy.h"
#include "entity.h"
#include "hud.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "particle.h"
#include "raymath.h"
#include "screen.h"
#include "wave_spawner.h"

Game game;

cpBool BulletvsAsteroidBegin(cpArbiter *arb, cpSpace *space, cpDataPointer userData)
{
  	cpShape *a, *b;
    cpArbiterGetShapes(arb, &a, &b);
	Bullet* bulletEntity = (Bullet*)cpShapeGetUserData(a);
    Asteroid* asteroidEntity = (Asteroid*)cpShapeGetUserData(b);
	
    bulletEntity->health--;
    if(bulletEntity->health <= 0){
    	bulletEntity->isAlive = 0;
    }
    asteroidEntity->isAlive = 0;
    AddScore(&game.hud,100);
    GainExperience(game.player,10);

    cpVect pos = cpBodyGetPosition(asteroidEntity->body);
	Vector2 randVel = {-100,100};
	Vector2 sizeBegin = {10,10};
	Emitter * e= AddEmitter((Vector2){pos.x,pos.y},Vector2Zero(),randVel,randVel,0.1,2,0.1,BROWN,BROWN,sizeBegin,Vector2Zero());
	e->particlesPerEmit = 10;

	Vector2 up = {0,-100};
	char text[32];
	sprintf_s(text,32,"%d",100);
	AddParticleText(text,(Vector2){pos.x,pos.y},up,24,32,0.7,WHITE);

	return cpTrue;
}

cpBool PlayerVsAnyBegin(cpArbiter *arb, cpSpace *space, cpDataPointer userData)
{
	cpShape *a, *b;
    cpArbiterGetShapes(arb, &a, &b);

    cpCollisionType type = cpShapeGetCollisionType(b); 

    if(type == PLAYER_BULLET_COLISSION_TYPE){return cpTrue;}
	Controller* player = (Controller*)cpShapeGetUserData(a);
	
	//printf("hurt\n");
	if(player->hurtAnimation){return cpTrue;}
	player->lastTimeHurt = GetTime();
	player->hurtAnimation = true;
	player->health--;
	
	return cpTrue;
}

cpBool EnemyVsAnyBegin(cpArbiter *arb, cpSpace *space, cpDataPointer userData)
{
	cpShape *a, *b;
    cpArbiterGetShapes(arb, &a, &b);
    cpCollisionType type = cpShapeGetCollisionType(b); 

	Enemy* enemy = (Enemy*)cpShapeGetUserData(a);

    if(type == PLAYER_BULLET_COLISSION_TYPE){
    	Bullet* bullet = (Bullet*)cpShapeGetUserData(b);
      	bullet->health--;
    	if(bullet->health <= 0){bullet->isAlive = 0;}
    	
    	enemy->health--;
    	if(enemy->health <= 0){
    		enemy->base.isAlive = 0;
    		GainExperience(game.player,enemy->expGiven);
			cpVect pos = cpBodyGetPosition(enemy->base.body);
			Vector2 randVel = {-100,100};
			Vector2 sizeBegin = {10,10};
			Emitter * e= AddEmitter((Vector2){pos.x,pos.y},Vector2Zero(),randVel,randVel,0.1,2,0.1,ORANGE,YELLOW,sizeBegin,Vector2Zero());
			e->particlesPerEmit = 10;
		 	AddScore(&game.hud,enemy->score);

	 		Vector2 up = {0,-100};
	 		char text[32];
	 		sprintf_s(text,32,"%d",enemy->score);
			AddParticleText(text,(Vector2){pos.x,pos.y},up,24,32,0.7,WHITE);

    	}
    }
	
	return cpTrue;
}


void InitGame()
{
	LoadResources();
 	game.space = cpSpaceNew();
 	game.player =  CreateController(cpv(300,300));
	game.hud =     CreateHud(&game.player->health);
	game.pause = false;
 	cpVect gravity = cpv(0,0);
	cpSpaceSetGravity(game.space, gravity);
	
	cpCollisionHandler *handlerBulletVsAsteroid = cpSpaceAddCollisionHandler(game.space, PLAYER_BULLET_COLISSION_TYPE, ASTEROID_COLISSION_TYPE);
	handlerBulletVsAsteroid->beginFunc = BulletvsAsteroidBegin;

	cpCollisionHandler* handlerPlayer =  cpSpaceAddWildcardHandler(game.space,PLAYER_COLLISION_TYPE);
	handlerPlayer->beginFunc = PlayerVsAnyBegin;

	cpCollisionHandler* handlerEnemy =  cpSpaceAddWildcardHandler(game.space,ENEMY_COLISSION_TYPE);
	handlerEnemy->beginFunc = EnemyVsAnyBegin;
	
	InitEnemyTypes();
	InitWaveSpawner(&game.spawner);
	Camera2D camera = { 0 };
	cpVect position = cpBodyGetPosition(game.player->base.body);
	camera.target = (Vector2){ position.x, position.y };
	camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 0.7f;
	game.camera = camera;
	game.showCardMenu = false;
	game.gameOver = false;

}

void DestroyGame()
{
	for (int i = 0; i < game.nAsteroids; ++i){DestroyEntity(game.asteroids[i]);}
	for (int i = 0; i < game.nBullets; ++i){  DestroyEntity(game.bullets[i]);}
	for (int i = 0; i < MAX_TEXTURES; ++i){UnloadTexture(game.textures[i]);}
	cpSpaceFree(game.space);
}

void AddAsteroid(Asteroid* a){
    if(game.nAsteroids == MAX_ASTEROIDS) {
    	DestroyEntity(a);
    	printf("max\n");
    	return;
    }
    game.asteroids[game.nAsteroids++] = a;
}
void AddEnemy(Enemy* e){
	if(game.nEnemys == MAX_ENEMYS) {
		DestroyEntity((Entity*)e);
		return;
	}
	game.enemys[game.nEnemys++] = e;
}
void AddBullet(Bullet* b){
	if(MAX_BULLETS == game.nBullets){ return;}
	game.bullets[game.nBullets++] = b;
}

void LoadResources()
{
	#define RESOURCES_PATH "resources/"
	#define AUDIO_PATH RESOURCES_PATH "Audio/"
	game.textures[IMG_SPACESHIP] = LoadTexture(RESOURCES_PATH "PlayerShip/playerShip1_blue.png");
	game.textures[IMG_SPACE_BACKGROUND] = LoadTexture(RESOURCES_PATH "black.png");
	game.textures[IMG_SPACE_LIFE] = LoadTexture(RESOURCES_PATH "playerLife3_blue.png");
	game.textures[IMG_CURSOR] = LoadTexture(RESOURCES_PATH "cursor.png");

	for (int i = 0; i < 5; ++i){
		game.textures[IMG_ENEMY_BLACK1 + i] = LoadTexture(TextFormat(RESOURCES_PATH "Enemies/enemyBlack%d.png",i+1));
	}

	for (int i = 0; i < 5; ++i){
		game.textures[IMG_ENEMY_BLUE1 + i] = LoadTexture(TextFormat(RESOURCES_PATH "Enemies/enemyBlue%d.png",i+1));
	}

	for (int i = 0; i < 5; ++i){
		game.textures[IMG_ENEMY_GREEN1 + i] = LoadTexture(TextFormat(RESOURCES_PATH "Enemies/enemyGreen%d.png",i+1));
	}

		for (int i = 0; i < 5; ++i){
		game.textures[IMG_ENEMY_ORANGE1 + i] = LoadTexture(TextFormat(RESOURCES_PATH "Enemies/enemyRed%d.png",i+1));
	}



	for (int i = 0; i < 4; ++i){
		game.textures[IMG_METEOR_BROWN_BIG + i] = LoadTexture(TextFormat(RESOURCES_PATH "Meteors/meteorBrown_big%d.png",i+1));
	}

	game.textures[IMG_BLACK_PANEL] = LoadTexture(RESOURCES_PATH "blank_panel.png");

	//LoadCardIcons
	game.textures[IMG_CARD_BIGGER_BULLET] = LoadTexture(RESOURCES_PATH "CardIcons/BiggerBullets.png");
	game.textures[IMG_CARD_BULLET_PER_SHOOT] = LoadTexture(RESOURCES_PATH "CardIcons/BulletHell.png");
	game.textures[IMG_CARD_FIRERATE] = LoadTexture(RESOURCES_PATH "CardIcons/FireRate.png");
	game.textures[IMG_CARD_INVICIBLETIME] = LoadTexture(RESOURCES_PATH "CardIcons/InvicibleTime.png");
	game.textures[IMG_CARD_LASERBEAM] =  LoadTexture(RESOURCES_PATH "CardIcons/LaserBeam.png");
	game.textures[IMG_CARD_PIERCING_SHOOT] = LoadTexture(RESOURCES_PATH "CardIcons/PiercingBullet.png");
	game.textures[IMG_CARD_REGEN] = LoadTexture(RESOURCES_PATH "CardIcons/Regen.png");
	game.textures[IMG_CARD_ROUNDBULLET] = LoadTexture(RESOURCES_PATH "CardIcons/RoundBullets.png");
	game.textures[IMG_CARD_SPEED] = LoadTexture(RESOURCES_PATH "CardIcons/Wing.png");
	game.textures[IMG_CARD_XPBOOST] = LoadTexture(RESOURCES_PATH "CardIcons/XPBoost.png");
	game.textures[IMG_CARD_MORE_HEALTH] = LoadTexture(RESOURCES_PATH "CardIcons/MoreHealth.png");
	game.textures[IMG_CARD_WASD] = LoadTexture(RESOURCES_PATH "CardIcons/WASD.png");

	//Load Bullets
	#define BULLETS_PATH RESOURCES_PATH "Bullets/"
	game.textures[IMG_BULLET_BLUE] = LoadTexture(BULLETS_PATH "bulletBlue.png");
	game.textures[IMG_BULLET_GREEN] = LoadTexture(BULLETS_PATH "bulletGreen.png");
	game.textures[IMG_BULLET_YELLOW] = LoadTexture(BULLETS_PATH "bulletYellow.png");
	game.textures[IMG_BULLET_RED] = LoadTexture(BULLETS_PATH "bulletRed.png");

	game.textures[IMG_BULLET_ORB_RED] = LoadTexture(BULLETS_PATH "orbBlue.png");
	game.textures[IMG_BULLET_ORB_GREEN] = LoadTexture(BULLETS_PATH "orbGreen.png");
	game.textures[IMG_BULLET_ORB_BLUE] = LoadTexture(BULLETS_PATH "orbRed.png");
	game.textures[IMG_BULLET_ORB_YELLOW] = LoadTexture(BULLETS_PATH "orbYellow.png");

	//LoadFonts
	game.fonts[FONT_UBUNTU_BOLD] = LoadFont(RESOURCES_PATH "Fonts/ubuntu.bold.ttf");
	game.fonts[FONT_UBUNTU_REGULAR] = LoadFont(RESOURCES_PATH "Fonts/ubuntu.bold.ttf");
	game.fonts[FONT_FASTUP] = LoadFont(RESOURCES_PATH "Fonts/04B_19__.TTF");

	//LoadAudio
	InitAudioDevice();
	game.sfx[AUDIO_LASER] = LoadSound(AUDIO_PATH "laserSmall_002.wav");
	game.sfx[AUDIO_THRUST] = LoadSound(AUDIO_PATH "thrusterFire_003.wav");

	for (int i = 0; i < MAX_SFX; ++i)
	{
		SetSoundVolume(game.sfx[i], 0.05);
	}

}

void DrawBackground()
{
	Texture2D background = game.textures[IMG_SPACE_BACKGROUND];

	int repetitionsX = ceilf((float)SCREEN_WIDTH / background.width)  ;
	int repetitionsY = ceilf((float)SCREEN_HEIGHT / background.height) ;

	float posX = 0;
	float posY = 0;
	for (int y = 0; y < repetitionsY; y++) {
	    for (int x = 0; x < repetitionsX; x++) {
	        DrawTexture(background, posX, posY, WHITE);
	        posX += background.width;
	    }
	    posX = 0;
	    posY += background.height;
	}

}

void SpawnAsteroids()
{
	int count = MAX_ASTEROIDS - game.nAsteroids;
	static float lastTimeSpawn = 0;
	for (int i = 0; i < count; ++i)
	{
		if(GetTime() > lastTimeSpawn){
			AddAsteroid(CreateAsteroid());
			lastTimeSpawn = GetTime() + 0.5f;
		}
	}
}




void IterateShapes(cpBody* body, cpShape *shape, void *data) {
    cpBB bb = cpShapeGetBB(shape);
    cpVect position = cpBodyGetPosition(body);

    // Calcula a largura e altura da forma usando a bounding box
    cpFloat width = bb.r - bb.l;
    cpFloat height = bb.t - bb.b;

    // Desenha a bounding box da forma
    DrawRectangleLines(position.x - width / 2, position.y - height / 2, width, height, WHITE);
}

void IterateBodies(cpBody *body, void *data) {cpBodyEachShape(body, IterateShapes, NULL);}
void DrawColliders(cpSpace *space) {cpSpaceEachBody(space, IterateBodies, NULL);}


void RestartGame(void)
{
	InitAtributes(game.player);
	InitWaveSpawner(&game.spawner);
	game.hud.realScore = game.hud.displayScore = 0;
	cpBodySetPosition(game.player->base.body, cpv((float)SCREEN_WIDTH/2,(float)SCREEN_HEIGHT/2));
	for (int i = 0; i < game.nAsteroids; ++i){DestroyEntity(game.asteroids[i]);}
	for (int i = 0; i < game.nBullets; ++i){DestroyEntity(game.bullets[i]);}
	for (int i = 0; i < game.nEnemys; ++i){DestroyEntity((Entity*)game.enemys[i]);}
	game.nAsteroids = 0;
	game.nBullets = 0;
	game.nEnemys = 0;
	game.showCardMenu = game.gameOver = game.pause = false;

}


void UpdateCollectionEntity(Entity** entities,int* nEnts,tpFunc updateEnt,tpFunc drawEnt,tpFunc destroyEnt)
{
	for (int index = 0; index < *nEnts; ++index)
	{
		Entity *ent = entities[index];

		if(ent->isAlive == 0){
			destroyEnt(entities[index]);
	        for (int o = index; o < *nEnts - 1; o++){
	            entities[o] = entities[o + 1];
	        }
	        *nEnts -= 1;
			index--;
			continue;
		}
		if(!game.pause){updateEnt(ent);}
		drawEnt(ent);
	}
}



void onPlayerLevelUP()
{
	game.pause = true;
	game.showCardMenu = true;
	game.currentCards[0] = CreateCard();
	game.currentCards[1] = CreateCard();
}


void DrawGame(void)
{
	if(game.player->health == 0){
		game.gameOver = true;
		game.pause = true;
	}

	if(!game.pause)
		cpSpaceStep(game.space, GetFrameTime());
	DrawBackground();
	SpawnAsteroids();

	UpdateWave(&game.spawner);
	BeginMode2D(game.camera);
	DrawParticles();
	UpdateCollectionEntity(game.bullets,&game.nBullets,UpdateEntity,DrawEntity,DestroyEntity);
	UpdateCollectionEntity(game.asteroids,&game.nAsteroids,UpdateAsteroid,DrawEntity,DestroyEntity);
	UpdateCollectionEntity((Entity**)game.enemys,&game.nEnemys,(tpUpdateEnt)UpdateEnemy,(tpFunc)DrawEnemy,DestroyEntity);

	if(!game.pause)
		UpdateController(game.player);

	DrawController(game.player);
	//DrawColliders(game.space);

	if(IsKeyPressed(KEY_F)){
		cpVect pos = cpBodyGetPosition(game.player->base.body);
		Vector2 up = {0,-140};
		AddParticleText("1000",(Vector2){pos.x,pos.y},up,24,32,0.7,WHITE);
	}

	EndMode2D();
}

#include "raygui.h"
void DrawGameGui(void)
{
	DrawHud(&game.hud);

	if(IsKeyPressed(KEY_ESCAPE)){
		game.pause = !game.pause;
	}

	if(game.gameOver)
	{
		Vector2 textSize = MeasureTextEx(GetFontDefault(), "GAME OVER", 128, 3);
		Vector2 textScoreSize = MeasureTextEx(GetFontDefault(), TextFormat("Score : %d",game.hud.realScore), 32, 0);
		Vector2 textRestart = MeasureTextEx(GetFontDefault(),"Press Enter to Restart",24,0);
		Vector2 position = (Vector2){SCREEN_WIDTH/2 -  textSize.x/2,SCREEN_HEIGHT/3};
		
		DrawTextEx(GetFontDefault(),"GAME OVER",position, 128,3, WHITE);
		position.y += textSize.y;
		DrawTextEx(game.fonts[FONT_UBUNTU_REGULAR],TextFormat("Score : %d",game.hud.realScore),position, 32,0, WHITE);
		position.y += textScoreSize.y;
		DrawTextEx(game.fonts[FONT_UBUNTU_REGULAR],"Press Enter to Restart",position, 24,0, WHITE);
		position.y += textRestart.y;
		DrawTextEx(game.fonts[FONT_UBUNTU_REGULAR],"Press ESCAPE to back menu",position, 24,0, WHITE);
	
		if(IsKeyPressed(KEY_ENTER)){RestartGame();}
		if(IsKeyPressed(KEY_ESCAPE)){
			RestartGame();
			ChangeScreen(SCREEN_TITLE);
		}
	}

	if(game.showCardMenu)
	{
		Vector2 textSize = MeasureTextEx(GetFontDefault(), "Choose a Card", 64, 6);
		Vector2 position = { (float)GetScreenWidth()/2 - textSize.x/2 , (float)GetScreenHeight() / 4};
		DrawText("Choose a Card",position.x,position.y, 64, WHITE);
		DrawCard(&game.currentCards[0],(Vector2){GetScreenWidth()/2 - (CARD_WIDTH+10),position.y + textSize.y + 10});
		DrawCard(&game.currentCards[1],(Vector2){GetScreenWidth()/2 + 10,position.y + textSize.y + 10});
	}

}
