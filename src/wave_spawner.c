#include "wave_spawner.h"
#include "enemy.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void InitWaveSpawner(WaveSpawner* wave)
{
	wave->currentWave = 0;
	wave->nEnemy = 0;
	wave->duration = 30;
	wave->spawnInterval = 0;
	GenerateWave(wave);
}

bool AddEnemyToSpawn(WaveSpawner* spawner,int type)
{
	if(spawner->nEnemy == MAX_ENEMIES_SPAWN){return true;}
	spawner->enemiesToSpawn[spawner->nEnemy++] = type;
	return false;
}

float calculateProgressionValue(int currentWave) {
    float initialValue = 10.0f;
    float growthRate = 1.25f;

    float add = (currentWave * currentWave) + 10;

      if(currentWave > 30){currentWave = 30;}

    // Calculating the value using the geometric progression formula
    float value = initialValue * powf(growthRate, currentWave - 1) + add;
    
    return value;
}
void GenerateWave(WaveSpawner* wave)
{
	wave->currentWave++;
	int startWaveValue,waveValue;
	startWaveValue = waveValue = calculateProgressionValue(wave->currentWave);

	while (waveValue > 0) {
		int type = rand() % MAX_ENEMY_TYPES;
		int cost = GetEnemyCost(type);
		if(cost > waveValue || cost > startWaveValue * 0.3){continue;}
		AddEnemyToSpawn(wave,type);
		waveValue -= cost;
	}	
	wave->spawnInterval = wave->duration / wave->nEnemy;
	printf("wave %d %f %d\n",wave->nEnemy,wave->duration,startWaveValue);
	wave->spawnTimer = wave->spawnInterval;
}
void UpdateWave(WaveSpawner* wave)
{
	if(game.pause){return;}
	if(wave->spawnTimer <= 0)
	{
		if(wave->nEnemy > 0){
		    int sizeIncreases = wave->currentWave / 30;    // Increase exp every 10 waves
		    float scaleMultiplier = 1;
		    if(sizeIncreases > 0)
		    {
	    	 	for (int i = 0; i < sizeIncreases; i++) {
		           scaleMultiplier *= 1.5;
		        }
		    }
			

			Enemy* e = AddEnemy(CreateEnemy(wave->enemiesToSpawn[wave->nEnemy-1],scaleMultiplier));


			if(e != NULL)
			{
			  	int bulletIncrements = wave->currentWave / 20; // Increment every 20 waves
			    int healthDoubles = wave->currentWave / 15;   //  health every 15 waves
			    int expIncreases = wave->currentWave / 10;    // Increase exp every 10 waves


			    e->bulletsPerShoot += bulletIncrements;

			    if (healthDoubles > 0) {
			        for (int i = 0; i < healthDoubles; i++) {
			            e->health *= 1.3;
			            e->maxHealth = e->health;
			        }
			    }



			    if (expIncreases > 0) {
			        for (int i = 0; i < expIncreases; i++) {
			            e->expGiven *= 2.0;
			        }
			    }
				
			}
			wave->nEnemy--;
			wave->spawnTimer = wave->spawnInterval;
		}else{
			wave->waveTimer = 0;
			GenerateWave(wave);
		}
	}else{
		wave->spawnTimer -= GetFrameTime();
	}

}
