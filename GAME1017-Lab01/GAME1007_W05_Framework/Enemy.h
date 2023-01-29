#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include <SDL_rect.h>
#include "Bullet.h"
#include <SDL_mixer.h>

const int BASE_ENEMY_WIDTH = 70;
const int BASE_ENEMY_HEIGHT = 70;

class Enemy {
public:
	// Enemy()
	Enemy(const float x, const float y, int speed); // Default AND non-default constructor
	void Update(double dt, Mix_Chunk*);
	SDL_Rect* GetSrc();
	SDL_FRect* GetDst();
private:
	SDL_Rect m_src;
	SDL_FRect m_dst;
	float m_speed;
	double m_dx;
	int m_bulletCtr, m_bulletMax;
};

#endif 