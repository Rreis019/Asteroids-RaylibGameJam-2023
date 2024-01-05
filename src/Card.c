#include "Card.h"
#include "raylib.h"
#include "game.h"
#include "text_bounds.h"
#include <stdio.h>
#include <stdlib.h>

//Effects
void EmptyEffect(){}
void BiggerBullet(){game.player->bulletSizeModifier += 0.5;}
void BulletHell(){game.player->bulletsPerShoot++;}
void FireRateBoost(){game.player->fireRateModifier += 0.50;}
void InvincibleTime(){game.player->fireRateModifier += 2.0;}
void PiercingBullet(){game.player->piercingBulletsCount++;}
void IncSpeed(){ game.player->speedModifier += 0.25;}
void IncXpBoost() {game.player->xpBoostModifier += 1.00;}
void IncMaxHealth(){
	game.player->maxHealth++;
	game.player->health++;
}
void SetWASD(){ game.player->wasd = true;}
void RegenHealth(){ game.player->health = game.player->maxHealth;}
void SpawnOrb(){ AddOrb(game.player);}

#define MAX_CARDS 11
Card cards[MAX_CARDS] = {
    (Card){.name = "Bigger Bullets", .description = "Increases bullet size by 50%", &game.textures[IMG_CARD_BIGGER_BULLET],.effect = BiggerBullet},
    (Card){.name = "Bullet Hell", .description = "Increases the number of bullets per shot", &game.textures[IMG_CARD_BULLET_PER_SHOOT],.effect = BulletHell},
    (Card){.name = "FireRate Boost", .description = "Boosts the rate of fire by 50%", &game.textures[IMG_CARD_FIRERATE],.effect = FireRateBoost},
    (Card){.name = "Invincible Time", .description = "Extends invincibility by 200% duration after being hit", &game.textures[IMG_CARD_INVICIBLETIME],.effect = InvincibleTime},
    //(Card){.name = "Laser Beam", .description = "Upgrades your weapon to a laser", &game.textures[IMG_CARD_LASERBEAM],.effect = EmptyEffect},
    (Card){.name = "Piercing Bullets", .description = "Allows bullets to pass through +1 enemy", &game.textures[IMG_CARD_PIERCING_SHOOT],.effect = PiercingBullet},
    (Card){.name = "Regen", .description = "Fully regenerates spaceship health", &game.textures[IMG_CARD_REGEN],.effect = RegenHealth},
    (Card){.name = "Round Bullets", .description = "Fires bullets that orbit the spaceship", &game.textures[IMG_CARD_ROUNDBULLET],.effect = SpawnOrb},
    (Card){.name = "Swift Wing", .description = "Enhances aircraft speed by 25%", &game.textures[IMG_CARD_SPEED],.effect = IncSpeed},
    (Card){.name = "XP Boost", .description = "Increases XP gained by 100%", &game.textures[IMG_CARD_XPBOOST],.effect = IncXpBoost},
 	(Card){.name = "WASD", .description = "Spaceship movement will be controlled using the WASD keys.", &game.textures[IMG_CARD_WASD],.effect = SetWASD},
 	(Card){.name = "More Health", .description = "Increases spaceship maximum health.", &game.textures[IMG_CARD_MORE_HEALTH],.effect = IncMaxHealth}
};


Card CreateCard()
{
	int r = rand();
	return cards[r % MAX_CARDS];
}

void DrawCard(Card* c,Vector2 position)
{
	Vector2 startPosition = position;

	Color borderColor = WHITE;
	if(CheckCollisionPointRec(GetMousePosition(),(Rectangle){startPosition.x,startPosition.y,CARD_WIDTH,CARD_HEIGHT})){
		borderColor = GREEN;
		if(IsMouseButtonPressed(0)){
			c->effect();
			game.pause = false;
			game.showCardMenu = false;
		}
	}

	//Border
	DrawRectangle(startPosition.x, startPosition.y, CARD_WIDTH, CARD_HEIGHT, BLACK);
	DrawRectangleLinesEx((Rectangle){position.x, position.y, CARD_WIDTH, CARD_HEIGHT}, 4,borderColor);

	//position.x += 36;
	position.y += 27;
	position.x = startPosition.x + CARD_WIDTH/2 - 50;
	
	//Icon
	DrawRectangleRounded((Rectangle){position.x,position.y,100,100}, 0.2, 13, WHITE);
	DrawTextureEx(*c->icon, position, 0,0.2, BLACK);
	position.y += 12 + 100;

	Vector2 titleSize = MeasureTextEx(game.fonts[FONT_UBUNTU_BOLD], c->name, 16, 0);
	DrawTextEx(game.fonts[FONT_UBUNTU_BOLD],c->name,(Vector2){startPosition.x + (CARD_WIDTH/2 - titleSize.x/2), position.y}, 16,0, WHITE);


	position.y += titleSize.y + 12;
	DrawLine(startPosition.x + 10,position.y, startPosition.x + CARD_WIDTH-10, position.y,WHITE);
	position.y += 12;

	DrawTextBoxed(
		game.fonts[FONT_UBUNTU_REGULAR],
		c->description,
		(Rectangle){
			startPosition.x + 10,
			position.y,
			CARD_WIDTH - 10,
			CARD_HEIGHT},
		16, 0,true,WHITE
	);
}



