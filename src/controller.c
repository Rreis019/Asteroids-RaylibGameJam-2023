#include "game.h"
#include "raylib.h"
#include "controller.h"

Controller* CreateController(cpVect position)
{
	Controller* c = malloc(sizeof(Controller));
	Entity* ent = (Entity*)c;
    ent->textHeight =  64;
	ent->textWidth  =  64;
	ent->texture = &game.textures[IMG_SPACESHIP];
    c->health = 3;
	ent->body  = cpSpaceAddBody(game.space,cpBodyNew(1.0, cpMomentForBox(1.0f,ent->textWidth,ent->textHeight)));
	cpBodySetPosition(ent->body, position);
	cpBodySetType(ent->body, CP_BODY_TYPE_DYNAMIC);
	cpBodySetAngle(ent->body, 0);
 	cpBodySetMoment(ent->body, INFINITY);
	ent->shape = cpSpaceAddShape(game.space,cpBoxShapeNew(ent->body,ent->textWidth,ent->textHeight,0));
    cpShapeSetElasticity(ent->shape, 0.8);
    cpShapeSetUserData(ent->shape,c);
    c->hurtAnimation = false;

    c->nextLevelXp = 150;
    c->experience = 0;
    c->level = 0;


	return c; 
}

cpVect GetBackPosition(Controller* c) {
    Entity* ent = (Entity*)c;
    cpVect frontPosition = cpBodyGetPosition(ent->body);

    // Obter o ângulo atual da nave
    cpFloat angle = cpBodyGetAngle(ent->body) - 90 * DEG2RAD;

    // Adicionar um deslocamento para a parte traseira da nave
    // Suponha que a nave está alinhada com o eixo X quando não está girando
    cpVect backOffset = cpvmult(cpvforangle(angle), -ent->textWidth * 0.5f);

    // Calcular e retornar a posição da parte traseira da nave
    return cpvadd(frontPosition, backOffset);
}

void UpdateController(Controller* c)
{
    Entity* ent = (Entity*)c;
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
    }

    // Limitar a velocidade máxima da nave
    #define MAX_SPEED 400
    float magnitude = cpvlength(cpBodyGetVelocity(ent->body));
    if (magnitude > MAX_SPEED){
        cpBodySetVelocity(ent->body, cpvmult(cpBodyGetVelocity(ent->body), MAX_SPEED / magnitude));
    }

    cpVect position = cpBodyGetPosition(c->base.body);
	game.camera.target = (Vector2){ position.x,position.y };
}

void DrawController(Controller* c)
{
	static bool draw = true;
	if(c->hurtAnimation && GetTime() > c->lastTimeHurt + INVICIBLE_TIME_SECONDS){
		c->hurtAnimation = false;	
		draw = true;
	}

	if(c->hurtAnimation){
		static float lastTimeSecond = 0;

		if(GetTime() > lastTimeSecond + 0.1){
			lastTimeSecond = GetTime();
			draw = !draw;
		}


	}

	if(draw){DrawEntity((Entity*)c);}
}

void GainExperience(Controller* c,int xp)
{
    c->experience += xp;

    if(c->experience > c->nextLevelXp){
        c->experience = c->experience -c->nextLevelXp;
        c->level++;
        c->nextLevelXp *= 1.2;
    }
}
