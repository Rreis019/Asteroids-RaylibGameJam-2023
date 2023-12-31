#include "enemy.h"
#include "chipmunk/chipmunk_types.h"
#include "chipmunk/cpVect.h"
#include "entity.h"
#include "game.h"
#include "raylib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
#define MAX_ENEMYS_TYPES 1
Enemy enemyType[MAX_ENEMYS_TYPES];

void InitEnemyTypes()
{
   enemyType[0] = 
}
*/

Enemy* CreateEnemy(int type)
{
	Enemy* e = malloc(sizeof(Enemy));
	*e = (Enemy){.base = CreateEntity((cpVect){100,100},IMG_ENEMY_BLACK1,0.5) ,.health = 1,.maxHealth = 100,.shootRange = 400.0f};
	//cpVect position = GetRandomPosition(e->base.textWidth,e->base.textHeight);
	//cpBodySetPosition(e->base.body, position);
	e->target = (Entity*)game.player;
	e->speed = 300;
  	e->fireRate = 3.0f;
  	e->lastTimeShoot = 0.0f;
  	e->bulletSpeed = 500.0f;
  	e->expGiven = 50;
  	cpShapeSetUserData(e->base.shape, e);
	cpBodySetMoment(e->base.body, INFINITY);
  	cpShapeSetCollisionType(e->base.shape,ENEMY_COLISSION_TYPE);
	return e;
}

void  UpdateEnemy(Enemy* e)
{
	//assert(e->base.body != NULL && "Entidade e invalida");
	if(e->target == NULL || e->base.body == NULL){return;}

	cpVect targetPosition = cpBodyGetPosition(e->target->body);
	cpVect position = cpBodyGetPosition(e->base.body);

	//calcular distancia
	double distance = cpvdist(targetPosition, position);

	cpVect direction = cpvsub(targetPosition, position);
   direction = cpvnormalize(direction);

   cpVect velocity = cpvmult(direction, e->speed);
	
	if(distance < e->shootRange){ 
		//Diminui a velocidade ate ficar a zero
		   double reductionFactor = distance / e->shootRange; // Fator de redução linear
        velocity = cpvmult(direction, e->speed * reductionFactor);
	}
  	
  	cpBodySetVelocity(e->base.body, velocity);
   double angle = atan2(direction.x, direction.y);
   cpBodySetAngle(e->base.body, -angle);

   if(GetTime() > e->lastTimeShoot + e->fireRate){
   	e->lastTimeShoot = GetTime();
   	Entity *ent = &e->base;
		cpFloat angle = cpBodyGetAngle(ent->body);
		float rotationRadians = angle +DEG2RAD * 90;
		cpVect bulletVelocity = cpBodyGetVelocity(ent->body);
		bulletVelocity.x += cos(rotationRadians) * e->bulletSpeed;
		bulletVelocity.y += sin(rotationRadians) * e->bulletSpeed;
		Bullet* b = CreateBullet(cpBodyGetPosition(ent->body), bulletVelocity, angle);
		cpShapeSetCollisionType(b->shape,ENEMY_BULLET_COLISSION_TYPE);
		AddBullet(b);
   }

}







