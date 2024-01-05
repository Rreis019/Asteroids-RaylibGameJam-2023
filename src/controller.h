#pragma once
#include "chipmunk/chipmunk.h"
#include "entity.h"
#include "particle.h"

#define INVICIBLE_TIME_SECONDS 0.3
#define MAX_ORBITS 100

typedef struct
{
	Entity base;
	int health;
	int maxHealth;
	bool hurtAnimation;
	float  lastTimeHurt;

	int level;
	float experience;
	float  nextLevelXp;

	float firerate;
	Emitter* em;

	//Controller Modifiers
	float bulletSizeModifier; //feito
	int bulletsPerShoot; //feito
	float fireRateModifier; //feito
	float invincibleTimeModifier; //feito
	//LaserBeam
	Bullet* orbsAroundTheShip[MAX_ORBITS];
	int nOrbs;

	int piercingBulletsCount; //feito
	float speedModifier; //feito
	float xpBoostModifier; //feito
	bool wasd;
}Controller;


void InitAtributes(Controller * c); //Resets atributes of controller
Controller* CreateController(cpVect position);
void 	   UpdateController(Controller* c);
void       DrawController(Controller* c);
void GainExperience(Controller* player,int xp);
cpVect GetBackPosition(Controller* c);
void AddOrb(Controller* c);