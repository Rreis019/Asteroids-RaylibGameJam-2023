#include "game.h"
#include "Card.h"
#include "chipmunk/chipmunk.h"
#include "controller.h"
#include "enemy.h"
#include "entity.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "particle.h"

Game game;

cpBool BulletvsAsteroidBegin(cpArbiter *arb, cpSpace *space, cpDataPointer userData)
{
  	cpShape *a, *b;
    cpArbiterGetShapes(arb, &a, &b);
	Bullet* bulletEntity = (Bullet*)cpShapeGetUserData(a);
    Asteroid* asteroidEntity = (Asteroid*)cpShapeGetUserData(b);
	
    bulletEntity->isAlive = 0;
    asteroidEntity->isAlive = 0;
    AddScore(&game.hud,100);
    GainExperience(game.player,50);
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
    	bullet->isAlive = false;

    	enemy->health--;
    	if(enemy->health <= 0){
    		enemy->base.isAlive = 0;
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
	
	Camera2D camera = { 0 };

	cpVect position = cpBodyGetPosition(game.player->base.body);

	camera.target = (Vector2){ position.x, position.y };
	camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	game.camera = camera;
	game.showCardMenu = false;
}

void DestroyGame()
{
	for (int i = 0; i < game.nAsteroids; ++i){DestroyEntity(game.asteroids[i]);}
	for (int i = 0; i < game.nBullets; ++i){  DestroyEntity(game.bullets[i]);}
	for (int i = 0; i < MAX_TEXTURES; ++i){UnloadTexture(game.textures[i]);}
	cpSpaceFree(game.space);
}

void AddAsteroid(Asteroid* a){
    if(game.nAsteroids == MAX_ASTEROIDS) {return;}
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
	game.textures[IMG_BULLET] = LoadTexture(RESOURCES_PATH "bulletBlue.png");
	game.textures[IMG_SPACE_BACKGROUND] = LoadTexture(RESOURCES_PATH "black.png");
	game.textures[IMG_SPACE_LIFE] = LoadTexture(RESOURCES_PATH "playerLife3_blue.png");
	game.textures[IMG_SPACE_SHIP_THURST] = LoadTexture(RESOURCES_PATH "thurst.png");
	
	for (int i = 0; i < 5; ++i){
		game.textures[IMG_ENEMY_BLACK1 + i] = LoadTexture(TextFormat(RESOURCES_PATH "Enemies/enemyBlack%d.png",i+1));
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

	//LoadFonts

	InitAudioDevice();
	game.sfx[AUDIO_LASER] = LoadSound(AUDIO_PATH "laserSmall_002.wav");
	game.sfx[AUDIO_THRUST] = LoadSound(AUDIO_PATH "thrusterFire_003.wav");

	for (int i = 0; i < MAX_SFX; ++i)
	{
		SetSoundVolume(game.sfx[i], 0.05);
	}

	game.fonts[FONT_UBUNTU_BOLD] = LoadFont(RESOURCES_PATH "Fonts/ubuntu.bold.ttf");
	game.fonts[FONT_UBUNTU_REGULAR] = LoadFont(RESOURCES_PATH "Fonts/ubuntu.bold.ttf");
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

void SpawnEnemy(Enemy* e)
{
   AddEnemy(e);

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
	game.player->health = 3;
	game.hud.realScore = game.hud.displayScore = 0;
	cpBodySetPosition(game.player->base.body, cpv((float)SCREEN_WIDTH/2,(float)SCREEN_HEIGHT/2));
	for (int i = 0; i < game.nAsteroids; ++i){DestroyEntity(game.asteroids[i]);}
	for (int i = 0; i < game.nBullets; ++i){DestroyEntity(game.bullets[i]);}
	for (int i = 0; i < game.nEnemys; ++i){DestroyEntity((Entity*)game.enemys[i]);}
	game.nAsteroids = 0;
	game.nBullets = 0;
	game.nEnemys = 0;
}


typedef void (*tpFunc)(Entity* ent);
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
	if(game.player->health == 0){RestartGame();}
	Camera2D * cam = &game.camera;

	if(!game.pause)
		cpSpaceStep(game.space, GetFrameTime());
	DrawBackground();
	SpawnAsteroids();

	BeginMode2D(game.camera);
	DrawParticles();
	UpdateCollectionEntity(game.bullets,&game.nBullets,UpdateEntity,DrawEntity,DestroyEntity);
	UpdateCollectionEntity(game.asteroids,&game.nAsteroids,UpdateAsteroid,DrawEntity,DestroyEntity);
	UpdateCollectionEntity((Entity**)game.enemys,&game.nEnemys,(tpUpdateEnt)UpdateEnemy,DrawEntity,DestroyEntity);
	

	if(!game.pause)
		UpdateController(game.player);



	DrawController(game.player);
	DrawColliders(game.space);

	if(IsKeyPressed(KEY_F)){
		SpawnEnemy(CreateEnemy(0));
	}

	EndMode2D();
  float scroll = GetMouseWheelMove(); // Obtém o movimento do scroll do mouse
    // Ajusta o zoom com base no movimento do scroll
    cam->zoom += scroll;

    // Limita o zoom a um valor mínimo e máximo (ajuste conforme necessário)
    if (cam->zoom < 0.7f) cam->zoom = 0.7f;
    if (cam->zoom > 1.0f) cam->zoom = 1.0f;
}

#include "raygui.h"
void DrawGameGui(void)
{
	DrawHud(&game.hud);

	if(IsKeyPressed(KEY_ESCAPE)){
		game.pause = !game.pause;
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
