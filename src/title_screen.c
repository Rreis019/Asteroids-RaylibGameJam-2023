#include "chipmunk/chipmunk_types.h"
#include "raylib.h"
#include "screen.h"
#include <stdio.h>
#include "game.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


void DrawTitleScreen()
{
	cpSpaceStep(game.space, GetFrameTime());
	DrawBackground();
	SpawnAsteroids();

	BeginMode2D(game.camera);
	UpdateCollectionEntity(game.asteroids,&game.nAsteroids,UpdateAsteroid,DrawEntity,DestroyEntity);
	EndMode2D();
}

bool init = false;
static Texture2D buttonText;



void DrawTitleScreenGui()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	#define MENU_BUTTON_WIDTH 220
	#define MENU_BUTTON_HEIGHT 60
	#define MARGIN 10

	int startX = screenWidth / 2 - MENU_BUTTON_WIDTH/2;
	int startY = screenHeight / 2 - MENU_BUTTON_HEIGHT/3;

	Rectangle rect = {startX,startY,MENU_BUTTON_WIDTH,MENU_BUTTON_HEIGHT};

	if(!init)
	{
		buttonText = LoadTexture("resources/ui/blue_button04.png");
		init = true;
	}


	static bool titleMoveUp = false;
	static float offset = 0;
	#define ANIMATION_SPEED 20
	#define MAX_OFFSET 30

	if(titleMoveUp){
		offset += ANIMATION_SPEED * GetFrameTime();
	}else {
		offset -= ANIMATION_SPEED * GetFrameTime();
	}

	if(offset >= MAX_OFFSET){
		offset = MAX_OFFSET;
		titleMoveUp = !titleMoveUp;
	}

	if(offset <= 0){
		offset = 0;
		titleMoveUp = !titleMoveUp;
	}

	HideCursor();



	Vector2 titleSize = MeasureTextEx(game.fonts[FONT_FASTUP],"AstroRaider",64,0);
	DrawTextEx(game.fonts[FONT_FASTUP],"AstroRaider",
		(Vector2){screenWidth/2 - titleSize.x/2, (startY - (titleSize.y+20)) - offset},
		64, 0,WHITE);
    

    GuiSetStyle(DEFAULT,TEXT_COLOR_NORMAL,ColorToInt(WHITE));
	GuiSetStyle(DEFAULT,TEXT_COLOR_FOCUSED,ColorToInt(WHITE));
	GuiSetStyle(DEFAULT,TEXT_COLOR_PRESSED,ColorToInt(WHITE));
	GuiSetStyle(DEFAULT,TEXT_SIZE,24);
	GuiSetFont(game.fonts[FONT_UBUNTU_REGULAR]);


	SetShapesTexture(buttonText,(Rectangle){0,0,buttonText.width,buttonText.height});

	if(GuiButton(rect, "Play")){
		ChangeScreen(SCREEN_GAMEPLAY);
	}
	rect.y += MENU_BUTTON_HEIGHT + MARGIN;

	GuiButton(rect, "Options");
	rect.y += MENU_BUTTON_HEIGHT + MARGIN;



	SetShapesTexture((Texture2D){ 0 }, (Rectangle){ 0 });
}