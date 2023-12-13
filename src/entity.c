#include "entity.h"
#include "game.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

SpaceShip CreateSpaceShip(float x,float y)
{
	SpaceShip s;
	s.textHeight = 64;
	s.textWidth =  64;
	s.position.x = x;
	s.position.y = y;
	s.velocity.x = s.velocity.y = 0;
	s.texture = &game.textures[IMG_SPACESHIP];
	s.rotation = 0;
	s.isAlive = 1;
	return s; 
}

void UpdateBullet(Bullet* b)
{
	b->position.x -= b->velocity.x * GetFrameTime();
	b->position.y -= b->velocity.y * GetFrameTime();

	if (b->position.x < 0 || b->position.x > SCREEN_WIDTH || b->position.y < 0 || b->position.y > SCREEN_HEIGHT){
		b->isAlive = 0;
	}
}

void UpdateSpaceShip(SpaceShip* s)
{
	s->position.x -= s->velocity.x * GetFrameTime();
	s->position.y -= s->velocity.y * GetFrameTime();

    if (s->position.x < 0) s->position.x = SCREEN_WIDTH;
    if (s->position.x > SCREEN_WIDTH) s->position.x = 0;
    if (s->position.y < 0) s->position.y = SCREEN_HEIGHT;
    if (s->position.y > SCREEN_HEIGHT) s->position.y = 0;

	if(IsKeyDown(KEY_A)){
		s->rotation-= 300 * GetFrameTime();
	}
	if(IsKeyDown(KEY_D)){
		s->rotation+= 300 * GetFrameTime();
	}

	if(IsKeyDown(KEY_SPACE))
	{
		static float lastTimeShoot = 0;
		if(GetTime() > lastTimeShoot + FIRE_RATE_SECONDS){
			lastTimeShoot = GetTime();

			Vector2 bulletVelocity;
			float rotationRadians = DEG2RAD * (s->rotation + 90);
			bulletVelocity.x = cos(rotationRadians) * 1000;
            bulletVelocity.y = sin(rotationRadians) * 1000;
			AddBullet(s->position,bulletVelocity,s->rotation);
		}
	}

	#define MAX_SPEED 400
	if(IsKeyDown(KEY_W))
	{
		float rotationRadians = DEG2RAD * (s->rotation + 90);
        s->velocity.x += cos(rotationRadians) * 20;
        s->velocity.y += sin(rotationRadians) * 20;

	}
    
    float magnitude = sqrt(s->velocity.x * s->velocity.x + s->velocity.y * s->velocity.y);

    if (magnitude > MAX_SPEED)
    {
        s->velocity.x = (s->velocity.x / magnitude) * MAX_SPEED;
        s->velocity.y = (s->velocity.y / magnitude) * MAX_SPEED;
    }


}


void DrawEntity(struct Entity* ent)
{
	Rectangle sourceRec = { 0.0f, 0.0f, ent->texture->width, ent->texture->height }; // Região completa da textura
    Rectangle destRec = { ent->position.x, ent->position.y, ent->textWidth, ent->textHeight };
    DrawTexturePro(*ent->texture, sourceRec, destRec, (Vector2){ ent->textWidth /2 , ent->textHeight /2 }, ent->rotation, WHITE);
}


