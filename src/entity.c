#include "entity.h"
#include "chipmunk/chipmunk.h"
#include "game.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

SpaceShip* CreateSpaceShip(cpVect position)
{
	SpaceShip* spaceShip = malloc(sizeof(SpaceShip));
	Entity* ent = (Entity*)spaceShip;
	ent->texture = &game.textures[IMG_SPACESHIP];
    ent->textHeight =  64;
	ent->textWidth  =  64;
    spaceShip->health = 3;
	ent->body  = cpSpaceAddBody(game.space,cpBodyNew(1.0, cpMomentForBox(1.0f,ent->textWidth,ent->textHeight)));
	cpBodySetPosition(ent->body, position);
	cpBodySetType(ent->body, CP_BODY_TYPE_DYNAMIC);
	cpBodySetAngle(ent->body, 0);
 	cpBodySetMoment(ent->body, INFINITY);
	ent->shape = cpSpaceAddShape(game.space,cpBoxShapeNew(ent->body,ent->textWidth,ent->textHeight,0));
    cpShapeSetElasticity(ent->shape, 0.8);
    cpShapeSetUserData(ent->shape,spaceShip);
    spaceShip->hurtAnimation = false;
	return spaceShip; 
}

Bullet* CreateBullet(cpVect position,cpVect velocity,cpFloat angle)
{
	Bullet* bullet = malloc(sizeof(Bullet));
	//b.velocity = velocity;
	//b.rotation = rotation;
	bullet->texture = &game.textures[IMG_BULLET];
	bullet->textWidth =  bullet->texture->width;
	bullet->textHeight = bullet->texture->height;
	bullet->isAlive = 1;

	bullet->body  = cpSpaceAddBody(game.space,cpBodyNew(1.0, cpMomentForBox(1.0f,bullet->textWidth,bullet->textHeight)));
	cpBodySetPosition(bullet->body, position);
	cpBodySetType(bullet->body, CP_BODY_TYPE_DYNAMIC);
	cpBodySetAngle(bullet->body,angle);
	bullet->shape = cpSpaceAddShape(game.space,cpBoxShapeNew(bullet->body,bullet->textWidth,bullet->textHeight,0));
	cpBodySetVelocity(bullet->body, velocity);
	cpShapeSetSensor(bullet->shape, 1);
  	cpShapeSetUserData(bullet->shape, bullet);
  	cpShapeSetCollisionType(bullet->shape,PLAYER_BULLET_COLISSION_TYPE);
	return bullet;
}

Asteroid* CreateAsteroid()
{
    Asteroid* asteroid = (Asteroid*)malloc(sizeof(Asteroid));
	cpSpace* space = game.space;
    #define RADIUS 5.0f
    asteroid->texture = &game.textures[IMG_METEOR_BROWN_BIG];
   	asteroid->textWidth = asteroid->texture->width;
   	asteroid->textHeight = asteroid->texture->height;
   	asteroid->isAlive = 1;
    asteroid->body = cpSpaceAddBody(space, cpBodyNew(1.0f, cpMomentForBox(1.0f,50.0f,50.0f)));
    asteroid->shape = cpSpaceAddShape(space, cpBoxShapeNew(asteroid->body,50,50,0));
    cpShapeSetFriction(asteroid->shape, 0.7); // Defina o coeficiente de atrito conforme necessário
   	cpBodySetMoment(asteroid->body, INFINITY);
   	cpBodySetAngle(asteroid->body, 0);
    cpShapeSetElasticity(asteroid->shape, 0.8);
  	cpShapeSetUserData(asteroid->shape, asteroid);
  	cpShapeSetCollisionType(asteroid->shape,ASTEROID_COLISSION_TYPE);
    // Definir a posição do asteroide com base na aleatoriedade
    cpVect asteroidPosition = cpv(0, 0);
    int side = rand() % 4;
    switch (side) {
        case 0: //TOP-SCREEN
            asteroidPosition.x = rand() % SCREEN_WIDTH;
            asteroidPosition.y -= RADIUS;
            break;
        case 1: //TOP-LEFT-SCREEN
            asteroidPosition.x = -RADIUS;
            asteroidPosition.y = rand() % SCREEN_HEIGHT;
            break;
        case 2: //TOP-RIGHT-SCREEN
            asteroidPosition.x = SCREEN_WIDTH + RADIUS;
            asteroidPosition.y = rand() % SCREEN_HEIGHT;
            break;
        case 3: //BOTTOM-SCREEN
            asteroidPosition.x = rand() % SCREEN_WIDTH;
            asteroidPosition.y = SCREEN_HEIGHT + RADIUS;
            break;
    }

    cpBodySetPosition(asteroid->body, asteroidPosition);
    // Definir a velocidade do asteroide
    cpVect asteroidVelocity = cpv(
    	(float)(rand() % (2 * ASTEROID_SPEED) - ASTEROID_SPEED), 
    	(float)(rand() % (2 * ASTEROID_SPEED) - ASTEROID_SPEED)
	);
    cpBodySetVelocity(asteroid->body, asteroidVelocity);
    return asteroid;
}

void UpdateEntity(Entity* b)
{
	if(b->body == NULL){return;}
    cpVect position = cpBodyGetPosition(b->body);
    cpFloat textWidth = b->textWidth * 2;
    cpFloat textHeight = b->textHeight * 2;

    cpFloat screenWidth = SCREEN_WIDTH + textWidth;
    cpFloat screenHeight = SCREEN_HEIGHT + textHeight;

    if (position.x < -textWidth*2 || position.x > screenWidth+textWidth ||
        position.y < -textHeight*2 || position.y > screenHeight+textHeight)
    {
        b->isAlive = 0;
        return;
    }
}

void UpdateSpaceShip(SpaceShip* spaceShip)
{
    Entity* ent = (Entity*)spaceShip;

    cpVect position = cpBodyGetPosition(ent->body);
    if (position.x < 0) cpBodySetPosition(ent->body, cpv(SCREEN_WIDTH, position.y));
    if (position.x > SCREEN_WIDTH) cpBodySetPosition(ent->body, cpv(0, position.y));
    if (position.y < 0) cpBodySetPosition(ent->body, cpv(position.x, SCREEN_HEIGHT));
    if (position.y > SCREEN_HEIGHT) cpBodySetPosition(ent->body, cpv(position.x, 0));

    cpFloat angle = cpBodyGetAngle(ent->body);
    // Atualizar a rotação do corpo baseado nos controles do jogador
    if (IsKeyDown(KEY_A)){
        angle -= DEG2RAD * 6;
        cpBodySetAngle(ent->body,  angle);
    }
    if (IsKeyDown(KEY_D)){
        angle += DEG2RAD * 6;
        cpBodySetAngle(ent->body, angle);
    }


	if (IsKeyDown(KEY_SPACE))
	{
	    static float lastTimeShoot = 0;
	    if (GetTime() > lastTimeShoot + FIRE_RATE_SECONDS)
	    {
	        lastTimeShoot = GetTime();

           	cpFloat angle = cpBodyGetAngle(ent->body);
	   		float rotationRadians = angle +DEG2RAD * 90;
	   		cpVect bulletVelocity = cpBodyGetVelocity(ent->body);
	        bulletVelocity.x += cos(rotationRadians) * -1000;
	        bulletVelocity.y += sin(rotationRadians) * -1000;
	        AddBullet(CreateBullet(cpBodyGetPosition(ent->body), bulletVelocity, angle));
	    }
	}

	#define THRUST_FORCE 30
	if (IsKeyDown(KEY_W))
	{
	    cpFloat angle = cpBodyGetAngle(ent->body);
   		float rotationRadians = angle +DEG2RAD * 90;
   		cpVect velocity = cpBodyGetVelocity(ent->body);
        velocity.x += cos(rotationRadians) * -THRUST_FORCE;
        velocity.y += sin(rotationRadians) * -THRUST_FORCE;
		cpBodySetVelocity(ent->body, velocity);


        Texture* fire = &game.textures[IMG_SPACE_SHIP_THURST];
        //DrawTexture(game.textures[IMG_SPACE_SHIP_THURST],position.x, position.y - cos(rotationRadians) * 100, WHITE);
	   Rectangle sourceRec = { 0.0f, 0.0f, fire->width, fire->height }; // Região completa da textura
        
        Vector2 flamePosition = { position.x, position.y + 100 }; 

        // Desenha o fogo com uma rotação igual à da nave
        DrawTexturePro(
            *fire,
            sourceRec,
            (Rectangle){ flamePosition.x, flamePosition.y, fire->width, fire->height },
            (Vector2){ (float)fire->width / 2, (float)fire->height / 2 },
            RAD2DEG * angle,
            WHITE
        );
    }

    // Limitar a velocidade máxima da nave
    #define MAX_SPEED 400
    float magnitude = cpvlength(cpBodyGetVelocity(ent->body));
    if (magnitude > MAX_SPEED){
        cpBodySetVelocity(ent->body, cpvmult(cpBodyGetVelocity(ent->body), MAX_SPEED / magnitude));
    }
}

void DrawEntity(Entity* ent)
{
	
	if(ent->body == NULL){return;}
	cpFloat angle = cpBodyGetAngle(ent->body);
	cpVect position = cpBodyGetPosition(ent->body);
	Rectangle sourceRec = { 0.0f, 0.0f, ent->texture->width, ent->texture->height }; // Região completa da textura
    Rectangle destRec = { position.x, position.y, ent->textWidth, ent->textHeight };
    DrawTexturePro(*ent->texture, sourceRec, destRec, (Vector2){ (float)ent->textWidth /2 , (float)ent->textHeight /2 }, RAD2DEG * angle, WHITE);
}


void DestroyEntity(Entity* ent) {
	//assert(ent->shape != NULL && "NULLO");
    cpSpaceRemoveShape(game.space, ent->shape);
 	cpSpaceRemoveBody(game.space, ent->body);
 	cpShapeFree(ent->shape);
 	cpBodyFree(ent->body);
 	free(ent);
}