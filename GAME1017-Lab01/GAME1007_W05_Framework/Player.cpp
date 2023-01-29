#include "Player.h"
#include <iostream>



Player::Player(float x, float y) : m_src({ 0,0,SPRITE_WIDTH,SPRITE_HEIGHT }), m_frameCtr(0), m_frameMax(1),
m_spriteIdx(0), m_spriteMin(0), m_spriteMax(1), m_state(PLR_IDLE)  // Initializers happen before body
{ // Body considered assignment not initialization
	std::cout << "Using our constructor" << std::endl;
	m_dst = { (x / 2.0f) - SPRITE_WIDTH, (y / 2.0f) - SPRITE_HEIGHT, m_src.w * 1.0f, m_src.h * 1.0f }; // x:- 324.17f y: - 196.17f
	m_spriteIdx = m_spriteMin;
}

void Player::Update() {
	// Animate
	if (m_frameCtr++ == m_frameMax) {
		m_frameCtr = 0;
		if (++m_spriteIdx == m_spriteMax) {
			m_spriteIdx = m_spriteMin;
		}
		m_src.x = m_src.w * m_spriteIdx;
	}
}

void Player::SetAnimation(plrState s, unsigned short min, unsigned short max) {
	m_state = s;
	m_spriteIdx = m_spriteMin = min;
	m_spriteMax = max;
	m_frameCtr = 0;
}

plrState Player::GetState() {
	return m_state;
}

SDL_Rect* Player::GetSrc() {
	return &m_src;
}

SDL_FRect* Player::GetDst() {
	return &m_dst;
}
