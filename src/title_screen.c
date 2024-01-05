#include "entity.h"
#include "raylib.h"
#include "raymath.h"
#include "screen.h"
#include <stdio.h>
#include <stdlib.h>
#include "chipmunk/chipmunk.h"
#include "game.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "particle.h"
void DrawScreenBackground()
{
	cpSpaceStep(game.space, GetFrameTime());
	DrawBackground();
	SpawnAsteroids();

	BeginMode2D(game.camera);
	UpdateCollectionEntity(game.asteroids,&game.nAsteroids,UpdateAsteroid,DrawEntity,DestroyEntity);
	


	if(IsMouseButtonDown(0))
	{
		for (int i = 0; i < game.nAsteroids; ++i)
		{
			Asteroid* asteroid = game.asteroids[i];
			cpBB bb = cpShapeGetBB(asteroid->shape);
	    	cpVect position = cpBodyGetPosition(asteroid->body);
		    cpFloat width = bb.r - bb.l;
		    cpFloat height = bb.t - bb.b;
		    Rectangle r = {position.x,position.y,width,height};

			Vector2 mouse = Vector2Subtract(GetMousePosition(), game.camera.offset);
			mouse = Vector2Scale(mouse, 1.0f / game.camera.zoom);
			mouse = Vector2Add(mouse, (Vector2){ game.camera.target.x, game.camera.target.y });
			if(CheckCollisionPointRec(mouse,r)){
				Vector2 randVel = {-100,100};
				Vector2 sizeBegin = {10,10};
				Emitter * e= AddEmitter((Vector2){position.x,position.y},Vector2Zero(),randVel,randVel,0.1,2,0.1,BROWN,BROWN,sizeBegin,Vector2Zero());
				e->particlesPerEmit = 10;
				asteroid->isAlive = false;
			}
		}
	}

	DrawParticles();
	EndMode2D();
}


Rectangle DrawTitle()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	#define MARGIN 10

	int startX = screenWidth / 2 - MENU_BUTTON_WIDTH/2;
	int startY = screenHeight / 2 - MENU_BUTTON_HEIGHT/3;

	Rectangle rect = {startX,startY,MENU_BUTTON_WIDTH,MENU_BUTTON_HEIGHT};

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

	return rect;
}


void DrawTitleScreenGui()
{
    Rectangle rect = DrawTitle();
    GuiSetStyle(DEFAULT,TEXT_COLOR_NORMAL,ColorToInt(WHITE));
	GuiSetStyle(DEFAULT,TEXT_COLOR_FOCUSED,ColorToInt(WHITE));
	GuiSetStyle(DEFAULT,TEXT_COLOR_PRESSED,ColorToInt(WHITE));
	GuiSetStyle(DEFAULT,TEXT_SIZE,24);
	GuiSetFont(game.fonts[FONT_UBUNTU_REGULAR]);
	
	SetShapesTexture(game.textures[IMG_BUTTON_BLUE],(Rectangle){0,0,game.textures[IMG_BUTTON_BLUE].width,game.textures[IMG_BUTTON_BLUE].height});

	if(GuiButton(rect, "Play")){
		ChangeScreen(SCREEN_GAMEPLAY);
		PlaySound(game.sfx[AUDIO_CLICK]);
		HideCursor();
	}
	rect.y += MENU_BUTTON_HEIGHT + MARGIN;

	if(GuiButton(rect, "Options")){
		ChangeScreen(SCREEN_OPTIONS);
		PlaySound(game.sfx[AUDIO_CLICK]);
	}

	rect.y += MENU_BUTTON_HEIGHT + MARGIN;

	if(GuiButton(rect, "Exit")){
		PlaySound(game.sfx[AUDIO_CLICK]);
		exit(0);
	}

	SetShapesTexture(game.textures[IMG_WHITE], (Rectangle){ 0,0,game.textures[IMG_WHITE].width,game.textures[IMG_WHITE].height });


}










