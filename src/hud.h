#pragma once

#include <stdbool.h>
#define MAX_LIFES 3

typedef struct
{
	int* playerHealth;
	int displayScore;
	int realScore;
	bool pulse;
}Hud;

Hud  CreateHud(int* playerHealth);
void AddScore(Hud* hud,int score);
void DrawHud(Hud* hud);