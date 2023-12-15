#pragma once
#include "chipmunk/chipmunk_types.h"
#include "raylib.h"
#include "chipmunk/chipmunk.h"

#define FIRE_RATE_SECONDS 0.1
#define BULLET_SPEED 1000
#define ASTEROID_SPEED 300

typedef struct Entity
{
	//float rotation;
	Texture2D* texture;
	int textWidth,textHeight;
	unsigned char isAlive;
	cpBody* body;
	cpShape* shape;
}Asteroid,SpaceShip,Bullet;

SpaceShip CreateSpaceShip(cpVect position);
void UpdateSpaceShip(SpaceShip* s);

void UpdateEntity(struct Entity* b);
void DrawEntity(struct Entity* ent);