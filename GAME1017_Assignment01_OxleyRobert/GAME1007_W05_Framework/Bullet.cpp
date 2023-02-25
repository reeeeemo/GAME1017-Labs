#include "Bullet.h"


Bullet::Bullet(const SDL_FPoint spawn, const float speed, bool fromPlayer) {

	m_isPlayerBullet = fromPlayer;
	m_dst = { spawn.x - 4.0f, spawn.y - 4.0f, 16.0f, 16.0f };
	this->m_speed = speed;
	// this-> is a pointer that refers to the object that gets constructed
	// It is optional. And should be if you choose your identifiers well.
	m_dx = 1.0;
	m_dy = 0.0;
}


void Bullet::Update(double dt) {
	m_dst.x += m_dx * m_speed * dt;
	//m_dst.y += m_dy * m_speed * dt; // No movement because m_dy is 0
}

double Bullet::GetXpos() {
	if (this->GetRect() != nullptr)
	{
		return m_dst.x;
	}
	return 0;
}

SDL_FRect* Bullet::GetRect() {
	return &m_dst;
}

bool Bullet::IsPlayerBullet() {
	return m_isPlayerBullet;
}