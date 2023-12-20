#include "game.h"
#include "entity.h"
#include "raylib.h"
#include <math.h>
#include "chipmunk/chipmunk.h"
#include <stdio.h>

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

	SpaceShip* player = (SpaceShip*)cpShapeGetUserData(a);
	
	printf("hurt\n");
	if(player->hurtAnimation){return cpTrue;}
	player->lastTimeHurt = GetTime();
	player->hurtAnimation = true;
	player->health--;
	
	return cpTrue;
}


void InitGame()
{
	LoadResources();
 	game.space = cpSpaceNew();
 	game.player =  CreateSpaceShip(cpv(300,300));
	game.hud =     CreateHud(&game.player->health);
	game.pause = false;
 	cpVect gravity = cpv(0,0);
	cpSpaceSetGravity(game.space, gravity);
	
	cpCollisionHandler *handlerBulletVsAsteroid = cpSpaceAddCollisionHandler(game.space, PLAYER_BULLET_COLISSION_TYPE, ASTEROID_COLISSION_TYPE);
	handlerBulletVsAsteroid->beginFunc = BulletvsAsteroidBegin;

	cpCollisionHandler* handlerPlayer =  cpSpaceAddWildcardHandler(game.space,PLAYER_COLLISION_TYPE);
	handlerPlayer->beginFunc = PlayerVsAnyBegin;
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

void AddBullet(Bullet* b){
	if(MAX_BULLETS == game.nBullets){ return;}
	game.bullets[game.nBullets++] = b;
}

void RemoveBullet(int index){
    if (index >= 0 && index < game.nBullets){
		DestroyEntity(game.bullets[index]);
        for (int i = index; i < game.nBullets - 1; i++){
            game.bullets[i] = game.bullets[i + 1];
        }
        game.nBullets--;
    }
}

void RemoveAsteroid(int index){
	if(index >= 0 && index < game.nAsteroids){
		DestroyEntity(game.asteroids[index]);
		for (int i = index; i < game.nAsteroids - 1; ++i){
			game.asteroids[i] = game.asteroids[i+1];
		}
		game.nAsteroids--;
	}
}



void LoadResources()
{
	game.textures[IMG_SPACESHIP] = LoadTexture("resources/PlayerShip/playerShip1_blue.png");
	game.textures[IMG_BULLET] = LoadTexture("resources/bulletBlue.png");
	game.textures[IMG_SPACE_BACKGROUND] = LoadTexture("resources/black.png");
	game.textures[IMG_SPACE_LIFE] = LoadTexture("resources/playerLife3_blue.png");
	game.textures[IMG_SPACE_SHIP_THURST] = LoadTexture("resources/thurst.png");
	for (int i = 0; i < 4; ++i)
	{
		game.textures[IMG_METEOR_BROWN_BIG + i] = LoadTexture(TextFormat("resources/Meteors/meteorBrown_big%d.png",i+1));
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
	game.player->health = 3;
	game.hud.realScore = game.hud.displayScore = 0;
	cpBodySetPosition(game.player->base.body, cpv((float)SCREEN_WIDTH/2,(float)SCREEN_HEIGHT/2));
	for (int i = 0; i < game.nAsteroids; ++i){DestroyEntity(game.asteroids[i]);}
	for (int i = 0; i < game.nBullets; ++i){DestroyEntity(game.bullets[i]);}
	game.nAsteroids = 0;
	game.nBullets = 0;
}

void DrawGame(void)
{
	if(game.player->health == 0){RestartGame();}

	cpSpaceStep(game.space, GetFrameTime());
	

	DrawBackground();
	SpawnAsteroids();
	for (int i = 0; i < game.nBullets; ++i)
	{
		Bullet* bullet = game.bullets[i];
		if(bullet->isAlive == 0){
			RemoveBullet(i);
			i--;
			continue;
		}

		UpdateEntity(bullet);
		DrawEntity(bullet);
	}


	for (int i = 0; i < game.nAsteroids; ++i){
		Asteroid* asteroid = game.asteroids[i];
		if(asteroid->isAlive == 0){
			RemoveAsteroid(i);
			i--;
			continue;
		}
		UpdateAsteroid(asteroid);
		DrawEntity(asteroid);
	}

	UpdateSpaceShip(game.player);

	static bool draw = true;
	if(game.player->hurtAnimation && GetTime() > game.player->lastTimeHurt + INVICIBLE_TIME_SECONDS){
		game.player->hurtAnimation = false;	
		draw = true;
	}

	if(game.player->hurtAnimation){
		static float lastTimeSecond = 0;

		if(GetTime() > lastTimeSecond + 0.1){
			lastTimeSecond = GetTime();
			draw = !draw;
		}


	}
		if(draw){DrawEntity((Entity*)game.player);}




	DrawColliders(game.space);

	//DrawFPS(30, 30);

	if(IsKeyPressed(KEY_F1)){
		for (int i = 0; i < game.nAsteroids; ++i)
		{
		}
	}

}

void DrawGameGui(void)
{
	DrawHud(&game.hud);
}
