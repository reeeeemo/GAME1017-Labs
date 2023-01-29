#pragma once
#include <SDL_rect.h>
#include<SDL_pixels.h>

class Bullet
{
public:
	Bullet(const SDL_FPoint spawn, const float speed, bool fromPlayer);
	void Update(double dt); // Bullets update themselves
	SDL_FRect* GetRect();
	double GetXpos();
	bool IsPlayerBullet();
	SDL_Color m_col;
	// void Render(); // What info would we need?
private:
	// SDL_Rect m_src; // You will need for M2.
	SDL_FRect m_dst; // Destination rectangle
	bool m_isPlayerBullet;
	float m_speed; // Speed of bullet, already *'d by deltaTime
	double m_dx, m_dy; // Changes in x and y coordinates per frame
};

