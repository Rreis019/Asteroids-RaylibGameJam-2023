#pragma once
#include "raylib.h"

#define FIRE_RATE_SECONDS 0.1
#define BULLET_SPEED 1000

typedef struct Entity
{
	Vector2 position;
	Vector2 velocity;
	float rotation;
	Texture2D* texture;
	int textWidth,textHeight;
	unsigned char isAlive;
}Asteroid,SpaceShip,Bullet;



void UpdateBullet(Bullet* b);

SpaceShip CreateSpaceShip(float x,float y);
void UpdateSpaceShip(SpaceShip* s);

void DrawEntity(struct Entity *ent);