#include "Bullet.h"

#include "CollisionManager.h"
#include "Engine.h"
#include "RenderManager.h"

Bullet::Bullet(const SDL_FRect dst, const double dx, const double dy):GameObject(dst), m_dx(dx), m_dy(dy) { }

void Bullet::Update()
{
	m_dst.x += (float)m_dx * s_speed * (float)Engine::Instance().GetDeltaTime();
	m_dst.y += (float)m_dy * s_speed * (float)Engine::Instance().GetDeltaTime();

	// Checks all four bounds to see if bullet is out of frame
	if ((m_dst.x > kWidth) || (m_dst.x < 0) || (m_dst.y > kHeight) || (m_dst.y < 0)) 
	{
		deleteMe = !deleteMe;
	}
}

void Bullet::Render()
{
	SDL_SetRenderDrawColor(REMA::Instance().GetRenderer(), 240, 255, 0, 255);
	SDL_RenderFillRectF(REMA::Instance().GetRenderer(), &m_dst);
}


const float Bullet::s_speed = 300.0f;
