#include "wave_spawner.h"
#include "enemy.h"
#include "raylib.h"
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

void GenerateWave(WaveSpawner* wave)
{
	wave->currentWave++;
	int startWaveValue,waveValue;
	startWaveValue = waveValue = wave->currentWave * 10;

	while (waveValue > 0) {
		int type = rand() % MAX_ENEMY_TYPES;
		int cost = GetEnemyCost(type);
		if(cost > waveValue || cost > startWaveValue * 0.5){continue;}
		AddEnemyToSpawn(wave,type);
		waveValue -= cost;
	}	
	wave->spawnInterval = wave->duration / wave->nEnemy;
	wave->spawnTimer = wave->spawnInterval;
}
void UpdateWave(WaveSpawner* wave)
{
	if(game.pause){return;}
	if(wave->spawnTimer <= 0)
	{
		if(wave->nEnemy > 0){
			AddEnemy(CreateEnemy(wave->enemiesToSpawn[wave->nEnemy-1]));
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
