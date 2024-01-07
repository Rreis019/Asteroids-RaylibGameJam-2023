/*******************************************************************************************
*
*   raylib gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 5.0
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <time.h>

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

#include "game.h"
#include "screen.h"


//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif


// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static RenderTexture2D target = { 0 };  // Render texture to render our game
 

// TODO: Define global variables here, recommended to make them static

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame();      // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{
    srand(time(NULL));
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Astro Raider");
    SetExitKey(KEY_NULL);
    InitGame();
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);  

    while (!WindowShouldClose()) {
          UpdateMusicStream(game.musics[MUSIC_TELEPATH]);
        UpdateDrawFrame();
    }
#endif
    // De-Initialization
    UnloadRenderTexture(target);
    
    DestroyGame();
    CloseWindow();        // Close window and OpenGL context
    return 0;
}




//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
    BeginTextureMode(target);
        ClearBackground(BLACK);
        DrawCurrentScreen();
    EndTextureMode();
    #define MIN(a, b) (((a) < (b)) ? (a) : (b))
    float scale = MIN((float)GetScreenWidth()/SCREEN_WIDTH, (float)GetScreenHeight()/SCREEN_HEIGHT);
    // Render to screen (main framebuffer)
    BeginDrawing();
      ClearBackground(BLACK);     // Clear screen background
       DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)SCREEN_WIDTH*scale))*0.5f, (GetScreenHeight() - ((float)SCREEN_HEIGHT*scale))*0.5f,
                           (float)SCREEN_WIDTH*scale, (float)SCREEN_HEIGHT*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
       DrawCurrentScreenGui();
    
       #if defined(PLATFORM_DESKTOP)
        HideCursor();
        DrawTexture(game.textures[IMG_CURSOR], GetMouseX() - (game.textures[IMG_CURSOR].width),GetMouseY() - (game.textures[IMG_CURSOR].height), WHITE);
       #endif
    EndDrawing();
    //----------------------------------------------------------------------------------  
}