#include "entity.h"
#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_types.h"
#include "game.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

SpaceShip CreateSpaceShip(cpVect position)
{
	SpaceShip s;
	s.textHeight =  64;
	s.textWidth  =  64;
	s.texture = &game.textures[IMG_SPACESHIP];
	s.isAlive = 1;

	s.body  = cpSpaceAddBody(game.space,cpBodyNew(1.0, cpMomentForBox(1.0f,s.textWidth,s.textHeight)));
	cpBodySetPosition(s.body, position);
	cpBodySetType(s.body, CP_BODY_TYPE_DYNAMIC);
	cpBodySetAngle(s.body, 0);
	s.shape = cpSpaceAddShape(game.space,cpBoxShapeNew(s.body,s.textWidth,s.textHeight,0));
	return s; 
}

void UpdateEntity(struct Entity* b)
{
    cpVect position = cpBodyGetPosition(b->body);
    cpFloat textWidth = b->textWidth * 2;
    cpFloat textHeight = b->textHeight * 2;

    cpFloat screenWidth = SCREEN_WIDTH + textWidth;
    cpFloat screenHeight = SCREEN_HEIGHT + textHeight;

    if (position.x < -textWidth || position.x > screenWidth ||
        position.y < -textHeight || position.y > screenHeight)
    {
        b->isAlive = 0;
        return;
    }
}

void UpdateSpaceShip(SpaceShip* s)
{
    //cpVect velocity = cpBodyGetVelocity(s->body);
//    cpBodySetPosition(s->body, cpvadd(cpBodyGetPosition(s->body), velocity));

    cpVect position = cpBodyGetPosition(s->body);
    if (position.x < 0) cpBodySetPosition(s->body, cpv(SCREEN_WIDTH, position.y));
    if (position.x > SCREEN_WIDTH) cpBodySetPosition(s->body, cpv(0, position.y));
    if (position.y < 0) cpBodySetPosition(s->body, cpv(position.x, SCREEN_HEIGHT));
    if (position.y > SCREEN_HEIGHT) cpBodySetPosition(s->body, cpv(position.x, 0));

    cpFloat angle = cpBodyGetAngle(s->body);
    // Atualizar a rotação do corpo baseado nos controles do jogador
    if (IsKeyDown(KEY_A)){
        angle -= DEG2RAD * 6;
        cpBodySetAngle(s->body,  angle);
    }
    if (IsKeyDown(KEY_D)){
        angle += DEG2RAD * 6;
        cpBodySetAngle(s->body, angle);
    }


	if (IsKeyDown(KEY_SPACE))
	{
	    static float lastTimeShoot = 0;
	    if (GetTime() > lastTimeShoot + FIRE_RATE_SECONDS)
	    {
	        lastTimeShoot = GetTime();

           	cpFloat angle = cpBodyGetAngle(s->body);
	   		float rotationRadians = angle +DEG2RAD * 90;
	   		cpVect bulletVelocity = cpBodyGetVelocity(s->body);
	        bulletVelocity.x += cos(rotationRadians) * -1000;
	        bulletVelocity.y += sin(rotationRadians) * -1000;

	        AddBullet(cpBodyGetPosition(s->body), bulletVelocity, angle);
	    }
	}
  
	#define THRUST_FORCE 20 

	if (IsKeyDown(KEY_W))
	{
	    cpFloat angle = cpBodyGetAngle(s->body);
   		float rotationRadians = angle +DEG2RAD * 90;
   		cpVect velocity = cpBodyGetVelocity(s->body);
        velocity.x += cos(rotationRadians) * -THRUST_FORCE;
        velocity.y += sin(rotationRadians) * -THRUST_FORCE;
		cpBodySetVelocity(s->body, velocity);
	}

    // Limitar a velocidade máxima da nave
    #define MAX_SPEED 400
    float magnitude = cpvlength(cpBodyGetVelocity(s->body));
    if (magnitude > MAX_SPEED)
    {
        cpBodySetVelocity(s->body, cpvmult(cpBodyGetVelocity(s->body), MAX_SPEED / magnitude));
    }
}

void DrawEntity(struct Entity* ent)
{
	cpFloat angle = cpBodyGetAngle(ent->body);
	cpVect position = cpBodyGetPosition(ent->body);
	Rectangle sourceRec = { 0.0f, 0.0f, ent->texture->width, ent->texture->height }; // Região completa da textura
    Rectangle destRec = { position.x, position.y, ent->textWidth, ent->textHeight };
    DrawTexturePro(*ent->texture, sourceRec, destRec, (Vector2){ ent->textWidth /2 , ent->textHeight /2 }, RAD2DEG * angle, WHITE);
}


