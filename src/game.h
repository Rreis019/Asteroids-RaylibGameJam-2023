#include "raylib.h"
#include "entity.h"
#include "hud.h"


#define MAX_ASTEROIDS 50
#define MAX_BULLETS 500

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef enum 
{
	IMG_SPACESHIP,
	IMG_BULLET,
	IMG_SPACE_BACKGROUND,
	MAX_TEXTURES
}IMAGE_ID;

typedef struct
{
	SpaceShip player;
	Asteroid asteroids[MAX_ASTEROIDS];
	int nAsteroids;

	Bullet bullets[MAX_BULLETS];
	int nBullets;

	Hud hud;
	Texture2D textures[MAX_TEXTURES];
}Game;

extern Game game;

void InitGame();
void AddAsteroid(Asteroid asteroid);
void AddBullet(Vector2 position,Vector2 velocity,float rotation);
void RemoveBullet(int index);

void LoadResources();
void DrawGame(void);
void DrawGui(void);

//void AddAsteroid(Asteroid a)
