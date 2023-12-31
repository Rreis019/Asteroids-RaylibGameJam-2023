#include "chipmunk/chipmunk_types.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
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

    c->firerate = 1.0f;

    c->nextLevelXp = 150;
    c->experience = 0;
    c->level = 0;
    c->em = AddEmitter(
	        (Vector2){ 0,0 },
	        (Vector2){ 0,0} ,
	        (Vector2){ 0, 0 },
	        (Vector2){ 0, 0 },
	        0.005, 0.5f, -1.0f, ORANGE, RED, (Vector2){ 10, 10 }, (Vector2){ 1, 1 });
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
    //cpFloat angle = cpBodyGetAngle(ent->body);
    // Atualizar a rotação do corpo baseado nos controles do jogador
   	Vector2 mousePosition = GetMousePosition(); // Obtém a posição atual do mouse

    // Calcular a direção do vetor entre a entidade e o mouse
    cpVect direction = cpvsub((cpVect){mousePosition.x,mousePosition.y}, (cpVect){GetScreenWidth()/2,GetScreenHeight()/2});

    // Calcular o ângulo em radianos usando atan2
    cpFloat angle = atan2(direction.y, direction.x);
    angle += 90 * DEG2RAD;

    // Converter para graus se necessário

    // Define o ângulo para olhar na direção do mouse
    cpBodySetAngle(ent->body, angle);

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
	    if (GetTime() > lastTimeShoot + c->firerate)
	    {
	        lastTimeShoot = GetTime();

           	cpFloat angle = cpBodyGetAngle(ent->body);
	   		float rotationRadians = angle +DEG2RAD * 90;
	   		cpVect bulletVelocity = cpBodyGetVelocity(ent->body);
	        bulletVelocity.x += cos(rotationRadians) * -1000;
	        bulletVelocity.y += sin(rotationRadians) * -1000;
	        AddBullet(CreateBullet(cpBodyGetPosition(ent->body), bulletVelocity, angle));
    	    PlaySound(game.sfx[AUDIO_LASER]);
	    }
	}

    


	#define THRUST_FORCE 30
	static float lastTimeW = -100;
	if (IsKeyDown(KEY_W))
	{
	    cpFloat angle = cpBodyGetAngle(ent->body);
   		float rotationRadians = angle +DEG2RAD * 90;
   		cpVect velocity = cpBodyGetVelocity(ent->body);
        velocity.x += cos(rotationRadians) * -THRUST_FORCE;
        velocity.y += sin(rotationRadians) * -THRUST_FORCE;
		cpBodySetVelocity(ent->body, velocity);


		cpVect v =  GetBackPosition(game.player);
    	c->em->active = true;
    	//Update Emiter position,rotation
	 	angle = cpBodyGetAngle(game.player->base.body);
	    rotationRadians = angle + DEG2RAD * 90;

	    Vector2 forwardVector = { cos(rotationRadians), sin(rotationRadians) };
	    cpVect leftVector;
	    leftVector.x = -forwardVector.y; // Girar 90 graus no sentido anti-horário
	    leftVector.y = forwardVector.x;

	    leftVector = cpvnormalize(leftVector);

	    c->em->position = (Vector2){ v.x, v.y };
	    c->em->startVelocity =       (Vector2){ forwardVector.x * 500, forwardVector.y * 500 };
	    c->em->randomPositionRangeX = (Vector2){ leftVector.x * -15,leftVector.x*  15 };
	    c->em->randomPositionRangeY = (Vector2){ leftVector.y * -15, leftVector.y * 15 };
	    
	    if(GetTime() > lastTimeW + 4.9f){
	    	PlaySound(game.sfx[AUDIO_THRUST]);
	    	lastTimeW = GetTime();
	    }
    }else{
    	c->em->active = false;
    	StopSound(game.sfx[AUDIO_THRUST]);
    	lastTimeW = -100;
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
        onPlayerLevelUP();
        printf("teste");
    }
}
