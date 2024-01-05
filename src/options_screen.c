#include "options_screen.h"
#include "screen.h"
#include "title_screen.h"
#include "raygui.h"
#include "game.h"
#include "raylib.h"




void DrawOptionsScreennGui()
{
	Rectangle rect = DrawTitle();
	SetShapesTexture(game.textures[IMG_BUTTON_BLUE],(Rectangle){0,0,game.textures[IMG_BUTTON_BLUE].width,game.textures[IMG_BUTTON_BLUE].height});

	rect.height = MENU_SLIDER_HEIGHT;
	
	if(GuiSlider(rect,"Audio",TextFormat("%1.f",game.audio* 120), &game.audio, 0, 1)){
		for (int i = 0; i < MAX_SFX; ++i){SetSoundVolume(game.sfx[i],game.audio);}
	
	}
	rect.height = MENU_BUTTON_HEIGHT;

	rect.y += MENU_SLIDER_HEIGHT + 10;

	if(GuiButton(rect,"Back")){
		ChangeScreen(SCREEN_TITLE);
		PlaySound(game.sfx[AUDIO_CLICK]);
	}
	SetShapesTexture(game.textures[IMG_WHITE], (Rectangle){ 0,0,game.textures[IMG_WHITE].width,game.textures[IMG_WHITE].height });
}
