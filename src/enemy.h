#pragma once
#include "entity.h"

typedef struct
{
	Entity base;
	Entity* target;
	int health;
	int maxHealth;
	int speed;
	int shootRange;
	float fireRate;
	float bulletSpeed;
	float lastTimeShoot;
	int expGiven;
}Enemy;

Enemy* CreateEnemy(int type);
void  UpdateEnemy(Enemy* e);
//Enemy DrawEnemy();


