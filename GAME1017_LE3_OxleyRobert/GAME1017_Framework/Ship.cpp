#include "Ship.h"
#include "Engine.h"
#include "AsteroidField.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "MathManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include <iostream>
using namespace std;

Ship::Ship(SDL_Rect src, SDL_FRect dst) :
	AnimatedSprite(src, dst, STATE_IDLING), m_thrust(0.5),
	m_angle(270), m_velMax(6.0), m_radius(38), m_rotSpeed(5.0)
{
	m_dx = m_dy = m_velX = m_velY = 0.0;
	m_center = { (m_dst.x + m_dst.w / 2.0f), (m_dst.y + m_dst.h / 2.0f) };
	SetAnimation(STATE_IDLING, 1, 0, 1);
}

Ship::~Ship()
{
	SOMA::StopSound(15);
}

void Ship::Update()
{
	// Rotate player. State-independent.
	if (EVMA::KeyHeld(SDL_SCANCODE_A))
	{
		m_angle -= m_rotSpeed; // Counter-Clockwsie
	} else if (EVMA::KeyHeld(SDL_SCANCODE_D))
	{
		m_angle += m_rotSpeed;
	}
	// Teleport ship. New requirement for Lab 3.
	if (EVMA::KeyPressed(SDL_SCANCODE_T))
	{
		TeleportShip();
	}
	// Spawn bullet.
	if (EVMA::MousePressed(1))
	{
		BulletPool* bulletStorage = dynamic_cast<BulletPool*>(STMA::CurrentState()->GetChild("bullets"));
		bulletStorage->Fire(GetCenter(), m_angle);
		SOMA::PlaySound("fire");
	}
	// Checking animation states.
	switch(m_state)
	{
	case STATE_IDLING:
		if (EVMA::KeyPressed(SDL_SCANCODE_W) || EVMA::KeyPressed(SDL_SCANCODE_SPACE)) // Transition check to moving
		{
			SetAnimation(STATE_MOVING, 2, 0, 4);
			SOMA::PlaySound("engines", -1, 15); // Reserving sound loop on channel 15.
		}
		break;
	case STATE_MOVING:
		m_dx = cos(MAMA::Deg2Rad(m_angle - 90));
		m_dy = sin(MAMA::Deg2Rad(m_angle - 90));
		// Update velocities
		m_velX += (m_dx * m_thrust);
		m_velY += (m_dy * m_thrust);
		m_velX = min(max(m_velX,-(m_velMax * fabs(m_dx))), (m_velMax * fabs(m_dx)));
		m_velY = min(max(m_velY, -(m_velMax * fabs(m_dy))), (m_velMax * fabs(m_dy)));
		// If both movement keys are released
		if (EVMA::KeyReleased(SDL_SCANCODE_W) && EVMA::KeyReleased(SDL_SCANCODE_SPACE)) // Transition check to moving
		{
			SetAnimation(STATE_IDLING, 1, 0, 1);
			SOMA::StopSound(15);
		}
		break;
	}
	// Apply drag and finish movement.
	m_velX *= 0.975;
	m_velY *= 0.975;
	m_center.x += (float)m_velX;
	m_center.y += (float)m_velY;
	// Wrap on screen.
	if (m_center.x < -m_dst.w / 2) {
		m_center.x = kWidth + m_dst.w / 2;
	}
	else if (m_center.x > kWidth + m_dst.w / 2) {
		m_center.x = 0 - m_dst.w / 2;
	}

	if (m_center.y < -m_dst.h / 2) {
		m_center.y = kHeight + m_dst.h / 2;
	}
	else if (m_center.y > kHeight + m_dst.h / 2) {
		m_center.y = 0 - m_dst.h / 2;
	}
	// Update dest rectangle.
	m_dst.x = m_center.x - m_dst.w / 2;
	m_dst.y = m_center.y - m_dst.h / 2;
	// Invoke the animation.
	Animate();
}

void Ship::Render()
{
	SDL_RenderCopyExF(REMA::GetRenderer(), TEMA::GetTexture("sprites"),
		&m_src, &m_dst, m_angle, nullptr, SDL_FLIP_NONE);
}

const double& Ship::GetRadius()	{ return m_radius; }

void Ship::TeleportShip()
{
	// Get a reference to the asteroids vector from Asteroid Field.
	// Hints: Get the current state, invoke GetChild and get the asteroids
	//		  Search for a new center point, doing a collision check with asteroids with a wider radius
	//		  If you've found a clear center point, move the ship there
	AsteroidField* field = dynamic_cast<AsteroidField*>(STMA::CurrentState()->GetChild("field"));
	bool playerCollidedAsteroid = true;
	do
	{
		// Placing spaceship on a random space WITHIN the screen.
		m_center.x = (rand() % kWidth) - m_dst.w;
		m_center.y = (rand() % kHeight) - m_dst.h;
		if (m_center.x < 0)
		{
			m_center.x += m_dst.w;
		}
		if (m_center.y < 0)
		{
			m_center.y += m_dst.h;
		}

		for (Asteroid* asteroid : field->GetAsteroids())
		{
			playerCollidedAsteroid = COMA::CircleCircleCheck(GetCenter(), asteroid->GetCenter(), GetRadius(), asteroid->GetRadius());
			if (!playerCollidedAsteroid)
			{
				m_dst.x = m_center.x - m_dst.w / 2;
				m_dst.y = m_center.y - m_dst.h / 2;
				break;
			}
		}
	} while (playerCollidedAsteroid);
}
