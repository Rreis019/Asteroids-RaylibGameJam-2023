#include "screen.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


void DrawTitleScreen()
{

}
void DrawTitleScreenGui()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	GuiButton((Rectangle){screenWidth - (10+125),screenHeight - (10 + 35),125,35}, "Credits");

	#define MENU_BUTTON_WIDTH 220
	#define MENU_BUTTON_HEIGHT 60
	#define MARGIN 10

	int startX = screenWidth / 2 - MENU_BUTTON_WIDTH/2;
	int startY = screenHeight / 2 - MENU_BUTTON_HEIGHT/2;

	Rectangle rect = {startX,startY,MENU_BUTTON_WIDTH,MENU_BUTTON_HEIGHT};
	if(GuiButton(rect, "Play")){
		ChangeScreen(SCREEN_GAMEPLAY);
	}
	rect.y += MENU_BUTTON_HEIGHT + MARGIN;

	GuiButton(rect, "Options");
	rect.y += MENU_BUTTON_HEIGHT + MARGIN;

	GuiButton(rect, "Shop");
	rect.y += MENU_BUTTON_HEIGHT + MARGIN;
}