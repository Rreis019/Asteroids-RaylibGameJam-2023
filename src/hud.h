#pragma once

#define MAX_LIFES 3

typedef struct
{
	int lifes;
	int score;
}Hud;

Hud  CreateHud(void);
void DrawHud(Hud* hud);