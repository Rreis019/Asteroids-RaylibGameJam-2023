#pragma once

#include <stdbool.h>
#define MAX_LIFES 3
#define MAX_LEVEL 30

typedef struct
{
	int* playerHealth;
	int displayScore;
	int realScore;
	bool pulse;
	float xp;
}Hud;

Hud  CreateHud(int* playerHealth);
void AddScore(Hud* hud,int score);
void DrawHud(Hud* hud);