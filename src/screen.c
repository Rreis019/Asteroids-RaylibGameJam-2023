#include "screen.h"
#include "title_screen.h"
#include "options_screen.h"
#include "game.h"

ScreenIndex currentScreen = SCREEN_TITLE;

void ChangeScreen(ScreenIndex index){
	currentScreen = index;
}

ScreenIndex GetCurrentScreen()
{
	return currentScreen;
}

void DrawCurrentScreen()
{
	switch (currentScreen) {
		case SCREEN_TITLE:
			DrawScreenBackground();
			break;
		case SCREEN_GAMEPLAY:
			DrawGame();
			break;
		case SCREEN_OPTIONS:
			DrawScreenBackground();
			break;
		default:
			break;
	}
}

void DrawCurrentScreenGui()
{
	switch (currentScreen) {
		case SCREEN_TITLE:
			DrawTitleScreenGui();
			break;
		case SCREEN_GAMEPLAY:
			DrawGameGui();
			break;
		case SCREEN_OPTIONS:
			DrawOptionsScreennGui();
			break;
		default:

			break;
	}
}