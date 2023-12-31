#pragma once
#include "chipmunk/chipmunk.h"
#include "entity.h"
#include "particle.h"

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

	float firerate;

	Emitter* em;
}Controller;

Controller* CreateController(cpVect position);
void 	   UpdateController(Controller* c);
void       DrawController(Controller* c);
void GainExperience(Controller* player,int xp);
cpVect GetBackPosition(Controller* c);
