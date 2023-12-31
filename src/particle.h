#pragma once
#include <stdbool.h>
#include "raylib.h"

#define MAX_PARTICLES 2000
#define MAX_EMITTERS 100

typedef struct 
{
	Vector2 position;
	Vector2 velocity;
	Color colorBegin,colorEnd;
	Vector2 sizeBegin,sizeEnd;
	float creationTime;
	float lifespan;
	int gravity;
}Particle;

typedef struct 
{
	Vector2 position;
	float emissionRate;	
	float particleLifespan; // Default lifespan for particles
	Vector2 startVelocity; //Start velocity for particles

	Vector2 randomPositionRangeX;
	Vector2 randomPositionRangeY;
	Vector2 randomVelocityRangeX;
	Vector2 randomVelocityRangeY;
	Vector2 sizeBegin,sizeEnd;
	Color colorBegin;
	Color colorEnd;
	float lifeTime; //Life Time for Emitter
	float lastTimeEmit;
	float creationTime;
	float gravity;

	bool active;
}Emitter;

Emitter* AddEmitter(Vector2 position,Vector2 velocity, Vector2 randVelRangeX, Vector2 randVelRangeY,float emitRate, float particleLife,float emitLife,
                      Color colorBegin, Color colorEnd,
                      Vector2 sizeBegin,Vector2 sizeEnd);
void DrawParticles(void);


