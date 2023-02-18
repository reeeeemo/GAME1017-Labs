#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_
#include "GameObject.h"
#include "SDL_rect.h"

// These could be moved... perhaps to a PhysicsManager?
constexpr double kGrav = 2.0;
constexpr double kJumpForce = 25.0;

class PlatformPlayer : public AnimatedSprite
{
public: // Methods.
	PlatformPlayer(SDL_Rect src, SDL_FRect dst);
	void Update();
	void Render();
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double accel);
	void SetAccelY(double accel);
	bool IsGrounded();
	void SetGrounded(bool grounded);
	double GetVelX();
	double GetVelY();
	void SetX(float x);
	void SetY(float y);
private: // Properties.
	bool m_grounded, m_facingLeft;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav;
};

#endif