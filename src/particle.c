#include "particle.h"
#include "raylib.h"
#include "raymath.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

Emitter emitters[MAX_EMITTERS];
Particle particles[MAX_PARTICLES];
int nParticles = 0,nEmitters = 0;

Emitter* AddEmitter(Vector2 position,Vector2 velocity, Vector2 randVelRangeX, Vector2 randVelRangeY,float emitRate, float particleLife,float emitLife,
                      Color colorBegin, Color colorEnd,Vector2 sizeBegin,Vector2 sizeEnd){

    Emitter emitter;
    emitter.position = position;
    emitter.startVelocity = velocity;
    emitter.randomVelocityRangeX = randVelRangeX;
    emitter.randomVelocityRangeY = randVelRangeY;
    emitter.emissionRate = emitRate;
    emitter.lifeTime = emitLife;
    emitter.particleLifespan = particleLife;
    emitter.colorBegin = colorBegin;
    emitter.colorEnd = colorEnd;
    emitter.lastTimeEmit = 0;
    emitter.gravity = 0;
    emitter.creationTime = GetTime();
    emitter.sizeBegin = sizeBegin;
    emitter.sizeEnd = sizeEnd;
    emitter.active = true;
    if(nEmitters != MAX_EMITTERS){
    	emitters[nEmitters++] = emitter;
    	return &emitters[nEmitters - 1];
    }

    return NULL;
}

Color LerpColor(Color colorStart, Color colorEnd, float amount) {
    Color result;
    result.r = (unsigned char)Lerp(colorStart.r, colorEnd.r, amount);
    result.g = (unsigned char)Lerp(colorStart.g, colorEnd.g, amount);
    result.b = (unsigned char)Lerp(colorStart.b, colorEnd.b, amount);
    result.a = (unsigned char)Lerp(colorStart.a, colorEnd.a, amount);
    return result;
}


float RandomFloat(float min, float max) {return min + ((float)rand() / RAND_MAX) * (max - min);}

void UpdateEmitter(Emitter* emitter)
{
	if(GetTime() > emitter->lastTimeEmit + emitter->emissionRate)
	{
		emitter->lastTimeEmit = GetTime();
		if(nParticles != MAX_PARTICLES){
			Particle p;
			p.creationTime = GetTime();
			p.position = emitter->position;
			p.velocity = emitter->startVelocity;
			p.velocity.x += RandomFloat(emitter->randomVelocityRangeX.x,emitter->randomVelocityRangeX.y);
			p.velocity.y += RandomFloat(emitter->randomVelocityRangeY.x,emitter->randomVelocityRangeY.y);
			p.colorBegin = emitter->colorBegin;
			p.colorEnd = emitter->colorEnd;
			p.sizeBegin = emitter->sizeBegin;
			p.sizeEnd = emitter->sizeEnd;
			p.lifespan  = emitter->particleLifespan;
			p.gravity = emitter->gravity;
			particles[nParticles++] = p;
		}
	}
}

void DrawParticles(void)
{
	for (int i = 0; i < nEmitters; ++i){
		if(emitters[i].lifeTime < 0){continue;}
		if(GetTime() > emitters[i].creationTime + emitters[i].lifeTime){
			for (int o = i; o < nEmitters-1; ++o){emitters[o] = emitters[o+1];}
			nEmitters--;
			i--;
		}
	}

	for (int i = 0; i < nEmitters; ++i){
		if(emitters[i].active == false){continue;}
		UpdateEmitter(&emitters[i]);
	}

	for (int i = 0; i < nParticles; ++i)
	{
		Particle* p = &particles[i];

		if(GetTime() > p->creationTime + p->lifespan){
			for (int o = i; o < nParticles - 1; ++o){
				particles[o] = particles[o+1]; 					
			}
			nParticles--;
			i--;
			continue;
		}

		float lifePercentage = (GetTime() - p->creationTime) / p->lifespan;
        Color color = LerpColor(p->colorBegin,p->colorEnd,lifePercentage); 
        Vector2 size = Vector2Lerp(p->sizeBegin, p->sizeEnd, lifePercentage);

		p->velocity.y += p->gravity;

		p->position.x += p->velocity.x * GetFrameTime();
		p->position.y += p->velocity.y * GetFrameTime();
		DrawRectangle(p->position.x, p->position.y, size.x, size.y,color);
	}
}
