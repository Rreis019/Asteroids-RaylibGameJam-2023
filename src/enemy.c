#include "enemy.h"
#include "chipmunk/chipmunk_types.h"
#include "chipmunk/cpVect.h"
#include "entity.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void EnemySpawnBehaviour(Enemy* e);
void EnemyShootBehaviour(Enemy* e);

Enemy enemyType[MAX_ENEMY_TYPES];

void InitEnemyTypes()
{
   enemyType[ENEMY_TYPE_BLACK1] = (Enemy){
   	.health = 1,.maxHealth = 1,
   	.speed = 250,.shootRange = 200,.fireRate = 3.0f, .bulletSpeed = 500.0f,
   	.expGiven = 25,
   	.cost = 1,
   	.base.texture = &game.textures[IMG_ENEMY_BLACK1],
   	.scale = 0.5f,
   	.score = 200,
   	.bulletScale = 1.0f,
   	.bulletsPerShoot = 1,
   	.update = EnemyShootBehaviour,
   	.bulletTexture = &game.textures[IMG_BULLET_GREEN]
   };

   enemyType[ENEMY_TYPE_BLUE1] =  enemyType[ENEMY_TYPE_BLACK1];
   enemyType[ENEMY_TYPE_BLUE1].base.texture = &game.textures[IMG_ENEMY_BLUE1];

   enemyType[ENEMY_TYPE_GREEN1] =  enemyType[ENEMY_TYPE_BLACK1];
   enemyType[ENEMY_TYPE_GREEN1].base.texture = &game.textures[IMG_ENEMY_GREEN1];

   enemyType[ENEMY_TYPE_ORANGE1] =  enemyType[ENEMY_TYPE_BLACK1];
   enemyType[ENEMY_TYPE_ORANGE1].base.texture = &game.textures[IMG_ENEMY_ORANGE1];

   //------------------------------------------------------------------

   enemyType[ENEMY_TYPE_BLACK2] = (Enemy){
   	.health = 2,.maxHealth = 2,
   	.speed = 200,.shootRange = 200,.fireRate = 1.0f, .bulletSpeed = 500.0f,
   	.expGiven = 50,
   	.cost = 3,
   	.base.texture = &game.textures[IMG_ENEMY_BLACK2],
   	.scale = 0.7f,
   	.score = 500,
   	.bulletScale = 1.0f,
   	.bulletsPerShoot = 1,
   	.update = EnemyShootBehaviour,
   	.bulletTexture = &game.textures[IMG_BULLET_YELLOW]
   };

   enemyType[ENEMY_TYPE_BLUE2] =  enemyType[ENEMY_TYPE_BLACK2];
   enemyType[ENEMY_TYPE_BLUE2].base.texture = &game.textures[IMG_ENEMY_BLUE2];

   enemyType[ENEMY_TYPE_GREEN2] =  enemyType[ENEMY_TYPE_BLACK2];
   enemyType[ENEMY_TYPE_GREEN2].base.texture = &game.textures[IMG_ENEMY_GREEN2];

   enemyType[ENEMY_TYPE_ORANGE2] =  enemyType[ENEMY_TYPE_BLACK2];
   enemyType[ENEMY_TYPE_ORANGE2].base.texture = &game.textures[IMG_ENEMY_ORANGE2];

   //------------------------------------------------------------------

   enemyType[ENEMY_TYPE_BLACK3] = (Enemy){
   	.health = 10,.maxHealth = 10,
   	.speed = 200,.shootRange = 100,.fireRate = 0.3f, .bulletSpeed = 100.0f,
   	.expGiven = 100,
   	.cost = 20,
   	.base.texture = &game.textures[IMG_ENEMY_BLACK3],
   	.scale = 1.0f,
   	.score = 1000,
   	.bulletScale = 5.0f,
   	.bulletsPerShoot = 1,
	 	.update = EnemyShootBehaviour,
	 	.bulletTexture = &game.textures[IMG_BULLET_ORB_RED]
   };

   enemyType[ENEMY_TYPE_BLUE3] =  enemyType[ENEMY_TYPE_BLACK3];
   enemyType[ENEMY_TYPE_BLUE3].base.texture = &game.textures[IMG_ENEMY_BLUE3];

   enemyType[ENEMY_TYPE_GREEN3] =  enemyType[ENEMY_TYPE_BLACK3];
   enemyType[ENEMY_TYPE_GREEN3].base.texture = &game.textures[IMG_ENEMY_GREEN3];

   enemyType[ENEMY_TYPE_ORANGE3] =  enemyType[ENEMY_TYPE_BLACK3];
   enemyType[ENEMY_TYPE_ORANGE3].base.texture = &game.textures[IMG_ENEMY_ORANGE3];

   //------------------------------------------------------------------

   enemyType[ENEMY_TYPE_BLACK4] = (Enemy){
   	.health = 3,.maxHealth = 3,
   	.speed = 200,.shootRange = 100,.fireRate = 0.5f, .bulletSpeed = 500.0f,
   	.expGiven = 500,
   	.cost = 30,//.cost = 50,
   	.base.texture = &game.textures[IMG_ENEMY_BLACK4],
   	.scale = 0.6f,
   	.score = 10000,
   	.bulletScale = 3.0f,
   	.bulletsPerShoot = 3,
   	.update = EnemyShootBehaviour,
   	.bulletTexture = &game.textures[IMG_BULLET_YELLOW]
   };

   enemyType[ENEMY_TYPE_BLUE4] =  enemyType[ENEMY_TYPE_BLACK4];
   enemyType[ENEMY_TYPE_BLUE4].base.texture = &game.textures[IMG_ENEMY_BLUE4];

   enemyType[ENEMY_TYPE_GREEN4] =  enemyType[ENEMY_TYPE_BLACK4];
   enemyType[ENEMY_TYPE_GREEN4].base.texture = &game.textures[IMG_ENEMY_GREEN4];

   enemyType[ENEMY_TYPE_ORANGE4] =  enemyType[ENEMY_TYPE_BLACK4];
   enemyType[ENEMY_TYPE_ORANGE4].base.texture = &game.textures[IMG_ENEMY_ORANGE4];

   //------------------------------------------------------------------

   enemyType[ENEMY_TYPE_BLACK5] = (Enemy){
   	.health = 15,.maxHealth = 15,
   	.speed = 200,.shootRange = 100,.fireRate = 10.0f, .bulletSpeed = 500.0f,
   	.expGiven = 1000,
   	.cost = 60,
   	.base.texture = &game.textures[IMG_ENEMY_BLACK5],
   	.scale = 1.7f,
   	.bulletScale = 1.0f,
   	.bulletsPerShoot = 1,
   	.update = EnemySpawnBehaviour,
   	.bulletTexture = &game.textures[IMG_BULLET_YELLOW]
   };

   enemyType[ENEMY_TYPE_BLUE5] =  enemyType[ENEMY_TYPE_BLACK5];
   enemyType[ENEMY_TYPE_BLUE5].base.texture = &game.textures[IMG_ENEMY_BLUE5];

   enemyType[ENEMY_TYPE_GREEN5] =  enemyType[ENEMY_TYPE_BLACK5];
   enemyType[ENEMY_TYPE_GREEN5].base.texture = &game.textures[IMG_ENEMY_GREEN5];

   enemyType[ENEMY_TYPE_ORANGE5] =  enemyType[ENEMY_TYPE_BLACK5];
   enemyType[ENEMY_TYPE_ORANGE5].base.texture = &game.textures[IMG_ENEMY_ORANGE5];


}


Enemy* CreateEnemy(int type,int scale)
{
	Enemy* e = malloc(sizeof(Enemy));
	memcpy(e, &enemyType[type], sizeof(Enemy));
	e->base.isAlive = 1;
	e->base.textWidth = e->base.texture->width * e->scale * scale;
	e->base.textHeight = e->base.texture->height * e->scale * scale;

	cpVect position = GetRandomPosition(e->base.textWidth,e->base.textHeight);
	e->target = (Entity*)game.player;
  	e->lastTimeShoot = 0.0f;

	e->base.body = cpSpaceAddBody(game.space,cpBodyNew(1.0, cpMomentForBox(1.0f,e->base.textWidth,e->base.textHeight)));
	e->base.shape = cpSpaceAddShape(game.space,cpBoxShapeNew(e->base.body,e->base.textWidth,e->base.textHeight,0));
	cpBodySetPosition(e->base.body, position);
  	cpShapeSetUserData(e->base.shape, e);
	cpBodySetMoment(e->base.body, INFINITY);
  	cpShapeSetCollisionType(e->base.shape,ENEMY_COLISSION_TYPE);
	return e;
}

int GetEnemyCost(int type)
{
	return enemyType[type].cost;
}



void MovementEnemy(Enemy* e)
{
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
}

void EnemyShoot(Enemy* e)
{
	if(GetTime() > e->lastTimeShoot + e->fireRate){
   	e->lastTimeShoot = GetTime();
   	Entity *ent = &e->base;
		cpFloat angle = cpBodyGetAngle(ent->body);

	   float bulletAngle = (angle * RAD2DEG) - (( 4 * e->bulletsPerShoot)/2);
		for (int i = 0; i < e->bulletsPerShoot; ++i)
		{
			float rotationRadians = (bulletAngle+90) * DEG2RAD;
			cpVect bulletVelocity = cpBodyGetVelocity(ent->body);
			bulletVelocity.x += cos(rotationRadians) * e->bulletSpeed;
			bulletVelocity.y += sin(rotationRadians) * e->bulletSpeed;
			Bullet* b = CreateBullet(e->bulletTexture,cpBodyGetPosition(ent->body), bulletVelocity, angle,e->bulletScale);
			AddBullet(b);
			cpShapeSetCollisionType(b->shape,ENEMY_BULLET_COLISSION_TYPE);

			bulletAngle += 4*2;
		}
   }
}

void EnemyShootBehaviour(Enemy* e)
{
	//assert(e->base.body != NULL && "Entidade e invalida");
	if(e->target == NULL || e->base.body == NULL){return;}

	MovementEnemy(e);
	EnemyShoot(e);
}


void SpawnBabies(Enemy* e)
{
	if(GetTime() > e->lastTimeShoot + e->fireRate){
   	e->lastTimeShoot = GetTime();
   	cpVect myPosition = cpBodyGetPosition(e->base.body);
   	//Only spawn first tier of enemies


   	int type =  rand() % 5;
   	Enemy* e = CreateEnemy(type,1);
   	cpBodySetPosition(e->base.body, myPosition);
   	AddEnemy(e);
	}
}


void EnemySpawnBehaviour(Enemy* e)
{
	if(e->target == NULL || e->base.body == NULL){return;}
	MovementEnemy(e);
	SpawnBabies(e);
}


void UpdateEnemy(Enemy* e){
	e->update(e);
}

void DrawEnemy(Enemy* e) {
    DrawEntity((Entity*)e);

    // Draw Healthbar
    Entity* ent = (Entity*)e;
    if(e->health != e->maxHealth && e->health > 0) {
        cpVect position = cpBodyGetPosition(e->base.body);
        
        float percentage =  (float)e->health / (float)e->maxHealth;

        float barWidth = ent->textWidth * percentage;
        float barHeight = 10;
        float barX = position.x - barWidth / 2; 
        float barY = position.y - ent->textHeight - barHeight;

        DrawRectangle(barX, barY, barWidth, barHeight, GREEN);
        DrawRectangleLinesEx((Rectangle){barX, barY, ent->textWidth, barHeight},2, WHITE);
    }   
}
