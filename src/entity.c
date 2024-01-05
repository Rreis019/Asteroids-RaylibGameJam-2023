#include "entity.h"
#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_types.h"
#include "controller.h"
#include "game.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Entity CreateEntity(cpVect position,int image,float scale)
{
    Entity e;
    e.texture = &game.textures[image];
    e.textWidth = e.texture->width * scale;
    e.textHeight = e.texture->height * scale;
    e.isAlive = 1;

    e.body = cpSpaceAddBody(game.space,cpBodyNew(1.0, cpMomentForBox(1.0f,e.textWidth,e.textHeight)));
    e.shape = cpSpaceAddShape(game.space,cpBoxShapeNew(e.body,e.textWidth,e.textHeight,0));
    cpBodySetPosition(e.body, position);
    return e;
}

Bullet* CreateBullet(Texture* bulletTexture,cpVect position,cpVect velocity,cpFloat angle,float scale)
{
	Bullet* bullet = malloc(sizeof(Bullet));
	//b.velocity = velocity;
	//b.rotation = rotation;
	bullet->texture = bulletTexture;
	bullet->textWidth =  bullet->texture->width * scale;
	bullet->textHeight = bullet->texture->height * scale;
	bullet->isAlive = 1;
    bullet->health = 1;

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

double randomInRange(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}


cpVect GetRandomPosition(int textWidth,int textHeight)
{
    int side = rand() % 4;
    cpVect randomPosition;
    switch (side) {
        case 0: // TOP-SCREEN
            randomPosition.x = game.camera.target.x - rand() % SCREEN_WIDTH;
            randomPosition.y = game.camera.target.y - (SCREEN_HEIGHT / (2 * game.camera.zoom)) - textHeight;
            break;
        case 1: // TOP-LEFT-SCREEN
            randomPosition.x = game.camera.target.x - (SCREEN_WIDTH / (2 * game.camera.zoom)) - textWidth;
            randomPosition.y = game.camera.target.y -  rand() % SCREEN_HEIGHT;
            break;
        case 2: // TOP-RIGHT-SCREEN
            randomPosition.x = game.camera.target.x + (SCREEN_WIDTH / (2 * game.camera.zoom)) + textWidth;
            randomPosition.y = game.camera.target.y -  rand() % SCREEN_HEIGHT;
            break;
        case 3: // BOTTOM-SCREEN
            randomPosition.x = game.camera.target.x - rand() % SCREEN_WIDTH;
            randomPosition.y = game.camera.target.y + (SCREEN_HEIGHT / (2 * game.camera.zoom)) + textHeight;
            break;
    }

    return randomPosition;
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
    asteroid->body = cpSpaceAddBody(space, cpBodyNew(1.0f, cpMomentForBox(1.0f, 50.0f, 50.0f)));
    asteroid->shape = cpSpaceAddShape(space, cpBoxShapeNew(asteroid->body, 50, 50, 0));
    cpShapeSetFriction(asteroid->shape, 0.7); // Defina o coeficiente de atrito conforme necessário
    cpBodySetMoment(asteroid->body, INFINITY);
    cpBodySetAngle(asteroid->body, 0);
    cpShapeSetElasticity(asteroid->shape, 0.8);
    cpShapeSetUserData(asteroid->shape, asteroid);
    cpShapeSetCollisionType(asteroid->shape, ASTEROID_COLISSION_TYPE);
    // Definir a posição do asteroide com base na aleatoriedade
    cpVect asteroidPosition = cpv(0, 0);
    int side = rand() % 4;
    cpVect asteroidVelocity;

    asteroidPosition = GetRandomPosition(asteroid->textWidth,asteroid->textHeight);
    switch (side) {
        case 0: // TOP-SCREEN
            asteroidVelocity = cpv(0, ASTEROID_SPEED); // Mover para baixo (direção positiva y)
            break;
        case 1: // TOP-LEFT-SCREEN
            asteroidVelocity = cpv(ASTEROID_SPEED, ASTEROID_SPEED); // Mover para a direita e para baixo
            break;
        case 2: // TOP-RIGHT-SCREEN
            asteroidVelocity = cpv(-ASTEROID_SPEED, ASTEROID_SPEED); // Mover para a esquerda e para baixo
            break;
        case 3: // BOTTOM-SCREEN
            asteroidVelocity = cpv(0, -ASTEROID_SPEED); // Mover para cima (direção negativa y)
            break;
    }

    cpBodySetPosition(asteroid->body, asteroidPosition);
    cpBodySetAngle(asteroid->body, randomInRange(0, 2 * PI));
    cpBodySetVelocity(asteroid->body, asteroidVelocity);


    return asteroid;
}


bool isOutOfBound(Entity* b, Camera2D* cam) {
    if(b->body == NULL) {
        return false;
    }

    cpVect position = cpBodyGetPosition(b->body);
    cpFloat textWidth = b->textWidth * 2;
    cpFloat textHeight = b->textHeight * 2;

    cpFloat cameraHalfWidth = SCREEN_WIDTH / (2 * cam->zoom);
    cpFloat cameraHalfHeight = SCREEN_HEIGHT / (2 * cam->zoom);

    cpFloat cameraLeft = cam->target.x - cameraHalfWidth;
    cpFloat cameraRight = cam->target.x + cameraHalfWidth;
    cpFloat cameraBottom = cam->target.y - cameraHalfHeight;
    cpFloat cameraTop = cam->target.y + cameraHalfHeight;

    cpFloat entityLeft = position.x - textWidth;
    cpFloat entityRight = position.x + textWidth;   
    cpFloat entityBottom = position.y - textHeight;
    cpFloat entityTop = position.y + textHeight;

    return (entityRight < cameraLeft || entityLeft > cameraRight ||
            entityTop < cameraBottom || entityBottom > cameraTop);
}

void UpdateEntity(Entity* b)
{
    if(b->body == NULL) {
        return;
    }

    Camera2D* cam = &game.camera;

    if(isOutOfBound(b,cam)){
        b->isAlive = 0;
    }
}

void UpdateAsteroid(Entity* b)
{
    if(b->body == NULL){return;}
      cpFloat randomRotation = (cpFloat)rand() / RAND_MAX * 0.1f; // Variação de rotação, você pode ajustar esse valor
    cpFloat currentAngle = cpBodyGetAngle(b->body); // Obter o ângulo atual
    cpBodySetAngle(b->body, currentAngle + randomRotation); // Definir um novo ângulo somando a rotação aleatória

    Camera2D* cam = &game.camera;

    if(isOutOfBound(b,cam)){
        b->isAlive = 0;
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
    //printf("Entity destroy\n");
    cpSpaceRemoveShape(game.space, ent->shape);
 	cpSpaceRemoveBody(game.space, ent->body);
 	cpShapeFree(ent->shape);
 	cpBodyFree(ent->body);
 	free(ent);
}



