#pragma once
#include "entity.h"
#include "raylib.h"

typedef enum {

	//First Tier
	ENEMY_TYPE_BLACK1,
	ENEMY_TYPE_GREEN1,
	ENEMY_TYPE_ORANGE1,
	ENEMY_TYPE_BLUE1,



	ENEMY_TYPE_BLACK2,
	ENEMY_TYPE_BLUE2,
	ENEMY_TYPE_GREEN2,
	ENEMY_TYPE_ORANGE2,



	ENEMY_TYPE_BLACK3,
	ENEMY_TYPE_BLACK4,
	ENEMY_TYPE_BLACK5,
	ENEMY_TYPE_BLUE3,
	ENEMY_TYPE_BLUE4,
	ENEMY_TYPE_BLUE5,
	ENEMY_TYPE_GREEN3,
	ENEMY_TYPE_GREEN4,
	ENEMY_TYPE_GREEN5,
	ENEMY_TYPE_ORANGE3,
	ENEMY_TYPE_ORANGE4,
	ENEMY_TYPE_ORANGE5,
	MAX_ENEMY_TYPES
}ENEMY_TYPE;


typedef struct Enemy_
{
	Entity base;
	Entity* target;
	int health;
	int maxHealth;
	int speed;
	int shootRange;
	float fireRate;
	float bulletSpeed;
	int bulletsPerShoot;
	float lastTimeShoot;
	int expGiven;
	int score;
	int cost; //cost to spawn
	float scale;
	Texture* bulletTexture;
	float bulletScale;
	void (*update)(struct Enemy_*);
}Enemy;

void InitEnemyTypes();
Enemy* CreateEnemy(int type,int scale);
void UpdateEnemy(Enemy* e);
void DrawEnemy(Enemy* ent);
int GetEnemyCost(int type);



