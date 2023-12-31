#pragma once
#include "raylib.h"

#define CARD_WIDTH 187
#define CARD_HEIGHT 279

typedef void (*tpEffect)(void);
typedef struct 
{
	char name[100];
	char description[255];
	Texture* icon;
	int level;
	tpEffect effect;
}Card;

Card CreateCard();
void DrawCard(Card* c,Vector2 position);
