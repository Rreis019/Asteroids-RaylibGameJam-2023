#include "hud.h"
#include "raylib.h"
#include "game.h"

Hud  CreateHud(void)
{
	Hud hud;
	hud.lifes = 2; 
	hud.score = 0;
	return hud;
}


void DrawHud(Hud* hud)
{
   DrawText(TextFormat("%d",hud->score), 10, 10, 32, WHITE);
   
   #define MARGIN_BETWEEN_LIFES 5

   int startPositionX =  GetScreenWidth() - 10;
   startPositionX -= game.textures[IMG_SPACE_LIFE].width + MARGIN_BETWEEN_LIFES;
   int startPositionY =  10;

   for (int i = 0; i < MAX_LIFES; ++i)
   {
   		if(hud->lifes > i)
   		{
   			DrawTexture(game.textures[IMG_SPACE_LIFE], startPositionX, startPositionY, WHITE);
   		}else{
			DrawTexture(game.textures[IMG_SPACE_LIFE], startPositionX, startPositionY, GRAY);
   		}

   		startPositionX -= game.textures[IMG_SPACE_LIFE].width + MARGIN_BETWEEN_LIFES;
   }
}