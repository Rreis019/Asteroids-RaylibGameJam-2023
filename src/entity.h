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

enum AsteroidType
{
	BIG_ASTEROID,
	SMALL_ASTEROID
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
}SpaceShip;


SpaceShip* CreateSpaceShip(cpVect position);
Bullet* CreateBullet(cpVect position,cpVect velocity,cpFloat angle);
Asteroid* CreateAsteroid();

void UpdateSpaceShip(SpaceShip* s);

void UpdateEntity(Entity* b);
void DrawEntity(Entity* ent);
void DestroyEntity(Entity* ent);