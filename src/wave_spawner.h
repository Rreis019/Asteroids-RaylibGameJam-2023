#pragma once
#include "enemy.h"

#define MAX_ENEMIES_SPAWN 500

typedef struct
{
	float duration;
	int currentWave;	
	int enemiesToSpawn[MAX_ENEMIES_SPAWN];
	int nEnemy;
	float spawnInterval;
	float waveTimer;
	float spawnTimer;
	
}WaveSpawner;

void InitWaveSpawner(WaveSpawner* wave);
void GenerateWave(WaveSpawner* wave);
void UpdateWave(WaveSpawner* wave);