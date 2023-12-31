#pragma once
#include "Card.h"
#include "enemy.h"
#include "raylib.h"
#include "chipmunk/chipmunk.h"
#include "hud.h"
#include "controller.h"
#include "entity.h"

#define MAX_ASTEROIDS 10
#define MAX_BULLETS 500
#define MAX_ENEMYS 10

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
	IMG_SPACE_SHIP_THURST,
	IMG_ENEMY_BLACK1,
	IMG_ENEMY_BLACK2,
	IMG_ENEMY_BLACK3,
	IMG_ENEMY_BLACK4,
	IMG_ENEMY_BLACK5,
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
	MAX_TEXTURES
}IMAGE_ID;

typedef enum {
	FONT_UBUNTU_BOLD,
	FONT_UBUNTU_REGULAR,
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

	Card currentCards[2];
	bool pause;
	bool showCardMenu;
}Game;

extern Game game;

void InitGame();
void DestroyGame();

void AddAsteroid(Asteroid* asteroid);
void AddBullet(Bullet* bullet);
void RemoveBullet(int index);

void LoadResources();
void DrawGame(void);
void DrawGameGui(void);

void onPlayerLevelUP(void);

//void AddAsteroid(Asteroid a)
