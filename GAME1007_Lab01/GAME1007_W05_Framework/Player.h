#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <SDL_rect.h>
#include <SDL_mixer.h>
#include "Bullet.h"

const int SPRITE_WIDTH = 153;
const int SPRITE_HEIGHT = 93;

enum plrState {PLR_IDLE, PLR_MOVE};

class Player {
public:
	// Player()
	Player(float x = 0.0f, float y = 0.0f); // Default AND non-default constructor
	void Update();
	void SetAnimation(plrState, unsigned short, unsigned short);
	plrState GetState();
	SDL_Rect* GetSrc();
	SDL_FRect* GetDst();
private:
	plrState m_state; // State control values for animation
	SDL_Rect m_src;
	SDL_FRect m_dst;
	unsigned short m_frameCtr, // Counts the number of frames
		m_frameMax, // Number of frames each sprite is displayed for. Animation speed.
		m_spriteIdx, // Which sprite to display
		m_spriteMin, // Starting sprite index of animation
		m_spriteMax; // Ending sprite index plus 1
};

#endif