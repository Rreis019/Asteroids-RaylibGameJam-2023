#include "game.h"
#include "entity.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Game game;

void InitGame()
{
	LoadResources();
	game.hud =     CreateHud();
 	game.player =  CreateSpaceShip(300,300);
}

	void AddAsteroid()
	{
		if(game.nAsteroids == MAX_ASTEROIDS){return;}

		Asteroid asteroid;
		asteroid.texture = &game.textures[IMG_METEOR_BROWN_BIG + rand() % 4];
		asteroid.textWidth = asteroid.texture->width;
		asteroid.textHeight = asteroid.texture->height;
		asteroid.position.x = asteroid.position.y = 0;

		int side = rand() % 4;
		switch (side) {
			case 0: //TOP-SCREEN
			    asteroid.position.x = rand() % SCREEN_WIDTH;
			    asteroid.position.y -= asteroid.textHeight+1;
				break;
			case 1: //TOP-LEFT-SCREEN
				asteroid.position.x = 0;
				asteroid.position.y = rand() % SCREEN_HEIGHT;
				break;
			case 2: //TOP-RIGHT-SCREEN
				asteroid.position.x = SCREEN_WIDTH + asteroid.textWidth;
				asteroid.position.y = rand() % SCREEN_HEIGHT;
				break;

			case 3: //BOTTOM-SCREEN
			    asteroid.position.x = rand() % SCREEN_WIDTH;
				asteroid.position.y = SCREEN_HEIGHT + asteroid.textHeight;
				break;
		}

		game.asteroids[game.nAsteroids++] = asteroid;
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
	game.textures[IMG_SPACESHIP] = LoadTexture("resources/PlayerShip/playerShip1_blue.png");
	game.textures[IMG_BULLET] = LoadTexture("resources/bulletBlue.png");
	game.textures[IMG_SPACE_BACKGROUND] = LoadTexture("resources/black.png");
	game.textures[IMG_SPACE_LIFE] = LoadTexture("resources/playerLife3_blue.png");

	for (int i = 0; i < 4; ++i)
	{
		game.textures[IMG_METEOR_BROWN_BIG + i] = LoadTexture(TextFormat("resources/Meteors/meteorBrown_big%d.png",i+1));
	}

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

	for (int i = 0; i < game.nBullets; ++i)
	{
		Bullet* bullet = &game.bullets[i];
		if(bullet->isAlive == 0){
			RemoveBullet(i);
			i--;
			continue;
		}

		UpdateEntity(bullet);
		DrawEntity(bullet);
	}

	for (int i = 0; i < game.nAsteroids; ++i)
	{
		Asteroid* asteroid = &game.asteroids[i];
		DrawEntity(asteroid);
	}

	UpdateSpaceShip(&game.player);
	DrawEntity(&game.player);


}

void DrawGameGui(void)
{
	DrawHud(&game.hud);
}
