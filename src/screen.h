#pragma once
typedef enum { 
    SCREEN_TITLE, 
    SCREEN_GAMEPLAY,
    SCREEN_OPTIONS,
}ScreenIndex;

#define MENU_BUTTON_WIDTH 220
#define MENU_BUTTON_HEIGHT 60
#define MENU_SLIDER_HEIGHT 30

void DrawCurrentScreen();
void DrawCurrentScreenGui();
void ChangeScreen(ScreenIndex index);
ScreenIndex GetCurrentScreen();