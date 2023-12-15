#include "game.h"
#include "chipmunk/chipmunk_types.h"
#include "entity.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Game game;

void InitGame()
{
	LoadResources();
 	game.space = cpSpaceNew();
	game.hud =     CreateHud();
 	game.player =  CreateSpaceShip(cpv(300,300));
 	cpVect gravity = cpv(0,0);
	cpSpaceSetGravity(game.space, gravity);
}

void DestroyGame()
{
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		UnloadTexture(game.textures[i]);
	}

	cpSpaceFree(game.space);
}

void AddAsteroid()
{
    if(game.nAsteroids == MAX_ASTEROIDS) {return;}

    cpSpace* space = game.space;
    Asteroid a;
    #define RADIUS 5.0f
    a.body = cpSpaceAddBody(space, cpBodyNew(1.0f, cpMomentForCircle(1.0f, 0, RADIUS, cpvzero)));
    a.shape = cpSpaceAddShape(space, cpCircleShapeNew(a.body, RADIUS, cpvzero));
    cpShapeSetFriction(a.shape, 0.7); // Defina o coeficiente de atrito conforme necessário

    // Definir a posição do asteroide com base na aleatoriedade
    cpVect asteroidPosition = cpv(0, 0);
    int side = rand() % 4;
    switch (side) {
        case 0: //TOP-SCREEN
            asteroidPosition.x = rand() % SCREEN_WIDTH;
            asteroidPosition.y -= RADIUS;
            break;
        case 1: //TOP-LEFT-SCREEN
            asteroidPosition.x = -RADIUS;
            asteroidPosition.y = rand() % SCREEN_HEIGHT;
            break;
        case 2: //TOP-RIGHT-SCREEN
            asteroidPosition.x = SCREEN_WIDTH + RADIUS;
            asteroidPosition.y = rand() % SCREEN_HEIGHT;
            break;
        case 3: //BOTTOM-SCREEN
            asteroidPosition.x = rand() % SCREEN_WIDTH;
            asteroidPosition.y = SCREEN_HEIGHT + RADIUS;
            break;
    }

    cpBodySetPosition(a.body, asteroidPosition);

    // Definir a velocidade do asteroide
    cpVect asteroidVelocity = cpv((float)(rand() % (2 * ASTEROID_SPEED) - ASTEROID_SPEED), (float)(rand() % (2 * ASTEROID_SPEED) - ASTEROID_SPEED));
    cpBodySetVelocity(a.body, asteroidVelocity);

    // Armazenar informações do asteroide no jogo
    // GameAsteroid é uma estrutura que guarda informações do asteroide
    game.asteroids[game.nAsteroids++] = a;
}

void RemoveBullet(int index)
{
    if (index >= 0 && index < game.nBullets)
    {
        for (int i = index; i < game.nBullets - 1; i++)
        {
            game.bullets[i] = game.bullets[i + 1];
        }
        game.nBullets--;
    }
}

void RemoveAsteroid(int index){
	if(index >= 0 && index < game.nAsteroids){
		for (int i = index; i < game.nAsteroids - 1; ++i)
		{
			game.asteroids[i] = game.asteroids[i+1];
		}
		game.nAsteroids--;
	}

}


void AddBullet(cpVect position,cpVect velocity,cpFloat angle)
{
	if(MAX_BULLETS == game.nBullets){ return;}

	Bullet b;
	//b.velocity = velocity;
	//b.rotation = rotation;
	b.texture = &game.textures[IMG_BULLET];
	b.textWidth =  b.texture->width;
	b.textHeight = b.texture->height;
	b.isAlive = 1;

	b.body  = cpSpaceAddBody(game.space,cpBodyNew(1.0, cpMomentForBox(1.0f,b.textWidth,b.textHeight)));
	cpBodySetPosition(b.body, position);
	cpBodySetType(b.body, CP_BODY_TYPE_DYNAMIC);
	cpBodySetAngle(b.body,angle);
	b.shape = cpSpaceAddShape(game.space,cpBoxShapeNew(b.body,b.textWidth,b.textHeight,0));
	cpBodySetVelocity(b.body, velocity);
	cpShapeSetSensor(b.shape, 1);
	game.bullets[game.nBullets++] = b;
}


void LoadResources()
{
	game.textures[IMG_SPACESHIP] = LoadTexture("resources/PlayerShip/playerShip1_blue.png");
	game.textures[IMG_BULLET] = LoadTexture("resources/bulletBlue.png");
	game.textures[IMG_SPACE_BACKGROUND] = LoadTexture("resources/black.png");
	game.textures[IMG_SPACE_LIFE] = LoadTexture("resources/playerLife3_blue.png");

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
	for (int i = 0; i < MAX_ASTEROIDS; ++i)
	{
		AddAsteroid();
	}
}

#include "chipmunk/chipmunk.h"


void IterateShapes(cpBody* body, cpShape *shape, void *data) {
    cpBB bb = cpShapeGetBB(shape);
    cpVect position = cpBodyGetPosition(body);

    // Calcula a largura e altura da forma usando a bounding box
    cpFloat width = bb.r - bb.l;
    cpFloat height = bb.t - bb.b;

    // Desenha a bounding box da forma
    DrawRectangleLines(position.x - width / 2, position.y - height / 2, width, height, WHITE);
}

void IterateBodies(cpBody *body, void *data) {
    cpBodyEachShape(body, IterateShapes, NULL);
}

void DrawColliders(cpSpace *space) {
    cpSpaceEachBody(space, IterateBodies, NULL);
}


void DrawGame(void)
{
	cpSpaceStep(game.space, GetFrameTime());
	
	DrawBackground();
	//SpawnAsteroids();
	for (int i = 0; i < game.nBullets; ++i)
	{
		Bullet* bullet = &game.bullets[i];
		if(bullet->isAlive == 0){
			RemoveBullet(i);
			i--;
			continue;
		}

		UpdateEntity(bullet);
		DrawEntity(bullet);
	}

	for (int i = 0; i < game.nAsteroids; ++i)
	{
		Asteroid* asteroid = &game.asteroids[i];
		if(asteroid->isAlive == 0){
			RemoveAsteroid(i);
			i--;
			continue;
		}


		UpdateEntity(asteroid);
		DrawEntity(asteroid);
	}

	UpdateSpaceShip(&game.player);
	DrawEntity(&game.player);

	DrawColliders(game.space);

	DrawFPS(30, 30);
}

void DrawGameGui(void)
{
	DrawHud(&game.hud);
}
