#include "Enemy.h"
#include "Engine.h" // For static vector access
#include <iostream>

using namespace std;

Enemy::Enemy(float x, float y, int speed) 
{ // Body considered assignment not initialization
	m_src = { 0,0, 70, 70 };
	m_dst = { x + BASE_ENEMY_WIDTH, y, m_src.w * 1.0f, m_src.h * 1.0f}; 
	m_speed = speed;
	m_bulletCtr = 0;
	m_bulletMax = 60 + rand() % 121;
} 
void Enemy::Update(double dt, Mix_Chunk* shootSound) {
	m_dst.x += -1.0f * m_speed * dt;
	if (m_bulletCtr++ % m_bulletMax == 0) { // For each 2 seconds, spawn a new enemy
		Bullet* temp = (new Bullet({
			m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 },
			-400, false));
		temp->m_col = { (Uint8)(rand() % 256), (Uint8)(rand() % 256), (Uint8)(rand() % 256), 255 };
		Engine::m_enemyBulletVec.push_back(temp);
		Mix_PlayChannel(-1, shootSound, 0);
	}
}


SDL_Rect* Enemy::GetSrc() {
	return &m_src;
}

SDL_FRect* Enemy::GetDst() {
	return &m_dst;
}