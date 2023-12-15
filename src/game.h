#pragma once
#include "chipmunk/chipmunk_types.h"
#include "raylib.h"
#include "chipmunk/chipmunk.h"
#include "entity.h"
#include "hud.h"


#define MAX_ASTEROIDS 15
#define MAX_BULLETS 500

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef enum 
{
	IMG_SPACESHIP,
	IMG_BULLET,
	IMG_SPACE_BACKGROUND,
	IMG_SPACE_LIFE,
	IMG_METEOR_BROWN_BIG,
	IMG_METEOR_BROWN_SMALL = 8,
	MAX_TEXTURES
}IMAGE_ID;

typedef struct
{
	SpaceShip player;
	Asteroid asteroids[MAX_ASTEROIDS];
	int nAsteroids;

	Bullet bullets[MAX_BULLETS];
	int nBullets;

	cpSpace *space;

	Hud hud;
	Texture2D textures[MAX_TEXTURES];
}Game;

extern Game game;

void InitGame();
void DestroyGame();

void AddAsteroid();
void AddBullet(cpVect position,cpVect velocity,cpFloat angle);
void RemoveBullet(int index);

void LoadResources();
void DrawGame(void);
void DrawGameGui(void);

//void AddAsteroid(Asteroid a)
