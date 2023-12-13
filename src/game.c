#include "game.h"
#include "entity.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

Game game;

void InitGame()
{
	LoadResources();
 	game.player =  CreateSpaceShip(300,300);
 	
}

void AddAsteroid(Asteroid asteroid)
{
}

void RemoveBullet(int index)
{
    if (index >= 0 && index < game.nBullets)
    {
        for (int i = index; i < game.nBullets - 1; i++)
        {
            game.bullets[i] = game.bullets[i + 1];
        }
        game.nBullets--;
    }
}


void AddBullet(Vector2 position,Vector2 velocity,float rotation)
{
	if(MAX_BULLETS == game.nBullets){ return;}

	Bullet b;
	b.position = position;
	b.velocity = velocity;
	b.rotation = rotation;
	b.texture = &game.textures[IMG_BULLET];
	b.textWidth =  b.texture->width;
	b.textHeight = b.texture->height;
	b.isAlive = 1;

	game.bullets[game.nBullets++] = b;
}


void LoadResources()
{
	game.textures[IMG_SPACESHIP] = LoadTexture("resources/player.png");
	game.textures[IMG_BULLET] = LoadTexture("resources/bulletBlue.png");
	game.textures[IMG_SPACE_BACKGROUND] = LoadTexture("resources/black.png");
}

void DrawGame(void)
{
	Texture2D background = game.textures[IMG_SPACE_BACKGROUND];

	int repetitionsX = ceilf((float)SCREEN_WIDTH / background.width)  ;
	int repetitionsY = ceilf((float)SCREEN_HEIGHT / background.height) ;


	float posX = 0;
	float posY = 0;
	for (int y = 0; y < repetitionsY; y++) {
	    for (int x = 0; x < repetitionsX; x++) {
	        DrawTexture(background, posX, posY, WHITE);
	        posX += background.width;
	    }
	    posX = 0;
	    posY += background.height;
	}

	UpdateSpaceShip(&game.player);
	for (int i = 0; i < game.nBullets; ++i)
	{
		Bullet* bullet = &game.bullets[i];
		
		if(bullet->isAlive == 0){
			RemoveBullet(i);
			i--;
			continue;
		}

		UpdateBullet(bullet);
		DrawEntity(bullet);
	}
	DrawEntity(&game.player);
}

void DrawGui(void)
{

}
