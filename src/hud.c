#include "hud.h"
#include "raylib.h"
#include "game.h"
#include "raymath.h"
#include <stdbool.h>
#include <stdio.h>

Hud  CreateHud(int* playerHealth)
{
	Hud hud;
	hud.playerHealth = playerHealth; 
	hud.realScore = hud.displayScore = 0;
   hud.pulse = false;
	return hud;
}

bool growUp = false,growDown = false;
void AddScore(Hud* hud,int score)
{
   hud->realScore += score;
   growUp = true;
}


void DrawHud(Hud* hud)
{
   #define MIN_FONT_SIZE 32
   #define MAX_FONT_SIZE 48
   #define SPEED 500

   static float fontSize = MIN_FONT_SIZE;

   if(growUp)
   {
      fontSize += SPEED * GetFrameTime();
      if(fontSize >= MAX_FONT_SIZE){
         growUp = false;
         growDown = true;
         fontSize = MAX_FONT_SIZE;
         hud->displayScore = hud->realScore;
      }
   }
   else if(growDown)
   {
      fontSize -= SPEED  * GetFrameTime();
      if(fontSize <=  MIN_FONT_SIZE){
         growDown = false;
         growUp = false;
         fontSize = MIN_FONT_SIZE;
      }
   }


   DrawText(TextFormat("%d",hud->displayScore), 10, 10, fontSize, WHITE);
   
   #define MARGIN_BETWEEN_LIFES 5

   int startPositionX =  GetScreenWidth() - 10;
   startPositionX -= game.textures[IMG_SPACE_LIFE].width + MARGIN_BETWEEN_LIFES;
   int startPositionY =  10;

   for (int i = 0; i < MAX_LIFES; ++i)
   {
   		if(*hud->playerHealth > i)
   		{
   			DrawTexture(game.textures[IMG_SPACE_LIFE], startPositionX, startPositionY, WHITE);
   		}else{
			DrawTexture(game.textures[IMG_SPACE_LIFE], startPositionX, startPositionY, GRAY);
   		}

		 startPositionX -= game.textures[IMG_SPACE_LIFE].width + MARGIN_BETWEEN_LIFES;
   }
}