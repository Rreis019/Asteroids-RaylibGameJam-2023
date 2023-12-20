#pragma once
typedef enum { 
    SCREEN_LOGO = 0, 
    SCREEN_TITLE, 
    SCREEN_GAMEPLAY,
    SCREEN_OPTIONS,
    SCREEN_SHOP, 
    SCREEN_ENDING
}ScreenIndex;

void DrawCurrentScreen();
void DrawCurrentScreenGui();
void ChangeScreen(ScreenIndex index);
