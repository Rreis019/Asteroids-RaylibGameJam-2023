#pragma once
#include "chipmunk/chipmunk.h"
#include "Card.h"
#include "enemy.h"
#include "raylib.h"
#include "hud.h"
#include "controller.h"
#include "entity.h"
#include "wave_spawner.h"

#define MAX_ASTEROIDS 10
#define MAX_BULLETS 500
#define MAX_ENEMYS 100

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef enum 
{
	IMG_SPACESHIP,	
	IMG_CURSOR,
	IMG_SPACE_BACKGROUND,
	IMG_SPACE_LIFE,
	IMG_METEOR_BROWN_BIG,
	IMG_METEOR_BROWN_SMALL = 8,
	IMG_ENEMY_BLACK1,
	IMG_ENEMY_BLACK2,
	IMG_ENEMY_BLACK3,
	IMG_ENEMY_BLACK4,
	IMG_ENEMY_BLACK5,
	IMG_ENEMY_BLUE1,
	IMG_ENEMY_BLUE2,
	IMG_ENEMY_BLUE3,
	IMG_ENEMY_BLUE4,
	IMG_ENEMY_BLUE5,
	IMG_ENEMY_GREEN1,
	IMG_ENEMY_GREEN2,
	IMG_ENEMY_GREEN3,
	IMG_ENEMY_GREEN4,
	IMG_ENEMY_GREEN5,
	IMG_ENEMY_ORANGE1,
	IMG_ENEMY_ORANGE2,
	IMG_ENEMY_ORANGE3,
	IMG_ENEMY_ORANGE4,
	IMG_ENEMY_ORANGE5,
	IMG_BLACK_PANEL,
	IMG_CARD_BIGGER_BULLET,
	IMG_CARD_BULLET_PER_SHOOT,
	IMG_CARD_FIRERATE,
	IMG_CARD_INVICIBLETIME,
	IMG_CARD_LASERBEAM,
	IMG_CARD_PIERCING_SHOOT,
	IMG_CARD_REGEN,
	IMG_CARD_ROUNDBULLET,
	IMG_CARD_SPEED,
	IMG_CARD_XPBOOST,
	IMG_CARD_WASD,
	IMG_CARD_MORE_HEALTH,
	IMG_BULLET_ORB_BLUE,
	IMG_BULLET_ORB_GREEN,
	IMG_BULLET_ORB_RED,
	IMG_BULLET_ORB_YELLOW,
	IMG_BULLET_BLUE,
	IMG_BULLET_GREEN,
	IMG_BULLET_YELLOW,
	IMG_BULLET_RED,
	MAX_TEXTURES
}IMAGE_ID;

typedef enum {
	FONT_UBUNTU_BOLD,
	FONT_UBUNTU_REGULAR,
	FONT_FASTUP,
	MAX_FONTS
}FONT_ID;

typedef enum
{
	AUDIO_LASER,
	AUDIO_THRUST,
	MAX_SFX
}SFX_ID;

typedef struct
{
	Camera2D camera;
	Controller* player;
	Asteroid* asteroids[MAX_ASTEROIDS];
	Enemy* enemys[MAX_ENEMYS];
	Bullet* bullets[MAX_BULLETS];
	int nAsteroids;
	int nBullets;
	int nEnemys;

	cpSpace *space;
	Hud hud;

	Texture2D textures[MAX_TEXTURES];
	Sound  sfx[MAX_SFX];
	Font   fonts[MAX_FONTS];

	WaveSpawner spawner;

	Card currentCards[2];
	bool pause;
	bool gameOver;
	bool showCardMenu;
}Game;

extern Game game;

void InitGame();
void DestroyGame();

typedef void (*tpFunc)(Entity* ent);
void UpdateCollectionEntity(Entity** entities,int* nEnts,tpFunc updateEnt,tpFunc drawEnt,tpFunc destroyEnt);

void DrawBackground();
void SpawnAsteroids();
void AddAsteroid(Asteroid* asteroid);
void AddBullet(Bullet* bullet);
void AddEnemy(Enemy* e);
void RemoveBullet(int index);

void LoadResources();
void DrawGame(void);
void DrawGameGui(void);

void onPlayerLevelUP(void);

//void AddAsteroid(Asteroid a)
