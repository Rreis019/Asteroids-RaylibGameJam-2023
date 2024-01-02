#include "screen.h"
#include "title_screen.h"
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
			DrawTitleScreen();
			break;
		case SCREEN_GAMEPLAY:
			DrawGame();
			break;
		case SCREEN_ENDING:

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
		case SCREEN_ENDING:

			break;
		default:

			break;
	}
}