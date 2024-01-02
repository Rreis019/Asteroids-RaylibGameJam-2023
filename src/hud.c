#include <stdbool.h>
#include "raylib.h"
#include "hud.h"
#include "game.h"


#include "raygui.h"

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

float t = 0.5;
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

   for (int i = 0; i < game.player->maxHealth; ++i)
   {
   		if(*hud->playerHealth > i)
   		{
   			DrawTexture(game.textures[IMG_SPACE_LIFE], startPositionX, startPositionY, WHITE);
   		}else{
			DrawTexture(game.textures[IMG_SPACE_LIFE], startPositionX, startPositionY, GRAY);
   		}

		 startPositionX -= game.textures[IMG_SPACE_LIFE].width + MARGIN_BETWEEN_LIFES;
   }

   Rectangle bound;
   bound.width = GetScreenWidth() * 0.5;
   bound.height = 30;
   bound.x = (float)GetScreenWidth()/2 - bound.width/2;
   bound.y = GetScreenHeight() - (10+bound.height);
   GuiProgressBar(bound, "", "",&game.player->experience, 0,game.player->nextLevelXp);
   
   if(game.showCardMenu)
   {
      Color rainbowColor;
      rainbowColor.r = (unsigned char)(255 * sinf(GetTime()));
      rainbowColor.g = (unsigned char)(255 * sinf(GetTime() + 2));
      rainbowColor.b = (unsigned char)(255 * sinf(GetTime() + 4));
      rainbowColor.a = 255;
      Vector2 textSize = MeasureTextEx(GetFontDefault(), "LEVEL UP", 24, 0);
      DrawText(TextFormat("LEVEL UP",game.player->level + 1), bound.x + (bound.width/2 - textSize.x/2), bound.y - 30, 24, rainbowColor);
   }else{
      Vector2 textSize = MeasureTextEx(GetFontDefault(), TextFormat("LvL: %d",game.player->level + 1), 24, 0);
      DrawText(TextFormat("LvL: %d",game.player->level + 1), bound.x + (bound.width/2 - textSize.x/2), bound.y - 30, 24, WHITE);
   }


   DrawTextEx(game.fonts[FONT_UBUNTU_BOLD],TextFormat("Wave : %d",game.spawner.currentWave), (Vector2){10,50},24,0,WHITE);
   DrawTextEx(game.fonts[FONT_UBUNTU_BOLD],TextFormat("Enimies : %d",game.spawner.nEnemy), (Vector2){10,80},24,0,WHITE);

}