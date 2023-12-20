#pragma once
#include "chipmunk/chipmunk.h"
#include "entity.h"

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
}Controller;

Controller* CreateController(cpVect position);
void 	   UpdateController(Controller* c);
void       DrawController(Controller* c);
void GainExperience(Controller* player,int xp);
