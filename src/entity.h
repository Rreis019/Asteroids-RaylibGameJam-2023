#pragma once
#include "raylib.h"
#include "chipmunk/chipmunk.h"
#include <stdbool.h>

#define BULLET_SPEED 1000
#define ASTEROID_SPEED 200
#define INVICIBLE_TIME_SECONDS 0.3

enum CollisionType
{
	PLAYER_COLLISION_TYPE,
	ASTEROID_COLISSION_TYPE = 1,
	PLAYER_BULLET_COLISSION_TYPE,
	ENEMY_COLISSION_TYPE,
	ENEMY_BULLET_COLISSION_TYPE
};

typedef struct
{
	//float rotation;
	Texture2D* texture;
	int textWidth,textHeight;
	unsigned char isAlive;
	cpBody* body;
	cpShape* shape;
	int health;
	bool wasd;
}Asteroid,Bullet,Entity;

Entity CreateEntity(cpVect position,int imageID,float scale);
Bullet* CreateBullet(Texture* bulletTexture,cpVect position,cpVect velocity,cpFloat angle,float scale);
Asteroid* CreateAsteroid();
cpVect GetRandomPosition(int textWidth,int textHeight);

typedef void (*tpUpdateEnt)(Entity*);

void UpdateAsteroid(Entity* b);
void UpdateEntity(Entity* b);
void DrawEntity(Entity* ent);
void DestroyEntity(Entity* ent);