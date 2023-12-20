#pragma once
#include "raylib.h"
#include "chipmunk/chipmunk.h"
#include <stdbool.h>

#define FIRE_RATE_SECONDS 0.1
#define BULLET_SPEED 1000
#define ASTEROID_SPEED 300

enum CollisionType
{
	PLAYER_COLLISION_TYPE,
	ASTEROID_COLISSION_TYPE = 1,
	PLAYER_BULLET_COLISSION_TYPE
};

typedef struct
{
	//float rotation;
	Texture2D* texture;
	int textWidth,textHeight;
	unsigned char isAlive;
	cpBody* body;
	cpShape* shape;
}Asteroid,Bullet,Entity;


#define INVICIBLE_TIME_SECONDS 0.3

typedef struct
{
	Entity base;
	int health;
	bool hurtAnimation;
	float  lastTimeHurt;

	int level;
	float experience;
	float  nextLevelXp;
}SpaceShip;


void GainExperience(SpaceShip* player,int xp);

SpaceShip* CreateSpaceShip(cpVect position);
Bullet* CreateBullet(cpVect position,cpVect velocity,cpFloat angle);
Asteroid* CreateAsteroid();

void UpdateSpaceShip(SpaceShip* s);

void UpdateAsteroid(Entity* b);
void UpdateEntity(Entity* b);
void DrawEntity(Entity* ent);
void DestroyEntity(Entity* ent);