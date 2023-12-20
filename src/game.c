#include "game.h"
#include "chipmunk/chipmunk.h"
#include "entity.h"
#include "raylib.h"
#include <math.h>
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

	Controller* player = (Controller*)cpShapeGetUserData(a);
	
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
 	game.player =  CreateController(cpv(300,300));
	game.hud =     CreateHud(&game.player->health);
	game.pause = false;
 	cpVect gravity = cpv(0,0);
	cpSpaceSetGravity(game.space, gravity);
	
	cpCollisionHandler *handlerBulletVsAsteroid = cpSpaceAddCollisionHandler(game.space, PLAYER_BULLET_COLISSION_TYPE, ASTEROID_COLISSION_TYPE);
	handlerBulletVsAsteroid->beginFunc = BulletvsAsteroidBegin;

	cpCollisionHandler* handlerPlayer =  cpSpaceAddWildcardHandler(game.space,PLAYER_COLLISION_TYPE);
	handlerPlayer->beginFunc = PlayerVsAnyBegin;

	Camera2D camera = { 0 };

	cpVect position = cpBodyGetPosition(game.player->base.body);

	camera.target = (Vector2){ position.x, position.y };
	camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	game.camera = camera;
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
			lastTimeSpawn = GetTime() + 0.2f;
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
		updateEnt(ent);
		drawEnt(ent);
	}
}

#include "particle.h"
Emitter em;
void DrawGame(void)
{
	if(game.player->health == 0){RestartGame();}
	Camera2D * cam = &game.camera;
	cpSpaceStep(game.space, GetFrameTime());
	DrawBackground();
	SpawnAsteroids();

	BeginMode2D(game.camera);
	UpdateCollectionEntity(game.bullets,&game.nBullets,UpdateEntity,DrawEntity,DestroyEntity);
	UpdateCollectionEntity(game.asteroids,&game.nAsteroids,UpdateAsteroid,DrawEntity,DestroyEntity);

	UpdateController(game.player);
	DrawController(game.player);
	DrawColliders(game.space);

	EndMode2D();
	DrawParticles();

	  float scroll = GetMouseWheelMove(); // Obtém o movimento do scroll do mouse

    // Ajusta o zoom com base no movimento do scroll
    cam->zoom += scroll;

    // Limita o zoom a um valor mínimo e máximo (ajuste conforme necessário)
    if (cam->zoom < 0.7f) cam->zoom = 0.7f;
    if (cam->zoom > 1.0f) cam->zoom = 1.0f;


    if (IsKeyPressed(KEY_F))
    {
    	Color red = RED;
    	red.a = 0.0;
    	Emitter* e = AddEmitter(
    		(Vector2){SCREEN_WIDTH/2,SCREEN_HEIGHT/2},
    		(Vector2){0,-2000},
    		(Vector2){-200,200},
    		(Vector2){0,0},
    		 0.1,1.0,1.0f, ORANGE, red,(Vector2){20,20},(Vector2){105,105});
    	//e->gravity = 100;
    }


}

void DrawGameGui(void)
{
	DrawHud(&game.hud);
}
