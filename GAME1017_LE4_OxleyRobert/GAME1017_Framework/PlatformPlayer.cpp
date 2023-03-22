#include "PlatformPlayer.h"
#include "Engine.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include <cmath>
#include <iostream>
using namespace std;

const std::string plrSpriteFile = "../Assets/img/Player.png";
const std::string plrTextFile = "../Assets/dat/Player.txt";
const std::string plrKey = "player";

PlatformPlayer::PlatformPlayer(SDL_Rect src, SDL_FRect dst) :AnimatedSprite(src, dst, STATE_IDLING),
	m_grounded(false), m_facingLeft(false), m_maxVelX(9.0),
	m_maxVelY(kJumpForce), m_grav(kGrav), m_drag(0.8)
{
	TEMA::LoadSpriteMap(plrTextFile.c_str(), plrSpriteFile.c_str(), plrKey);
	SetSpriteSheet(TEMA::GetSpriteSheet(plrKey));

	//TEMA::Load("../Assets/img/player.png", "player");
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	SetAnimation(m_state, "idle"); // Initialize idle animation.
}

void PlatformPlayer::Update()
{
	// Checking states.
	switch (m_state)
	{
	case STATE_IDLING:
		// Transition to jump.
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			m_accelY = -kJumpForce; // SetAccelY(-JUMPFORCE);
			m_grounded = false; // SetGrounded(false);
			SOMA::PlaySound("jump");
			SetAnimation(STATE_JUMPING, "jump");
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) || EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			SetAnimation(STATE_RUNNING, "run");
		}
		break;
	case STATE_JUMPING:
		// Move left and right
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_accelX = -1.5;
			m_facingLeft = true;
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_accelX = 1.5;
			m_facingLeft = false;
		}

		// Hit the ground, transition to run.
		if (m_grounded)
		{
			SetAnimation(STATE_IDLING, "idle");
		}
		break;
	case STATE_RUNNING:
		// Move left and right
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_accelX = -1.5;
			m_facingLeft = true;
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_accelX = 1.5;
			m_facingLeft = false;
		}
		// Transition to jump.
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			m_accelY = -kJumpForce; // SetAccelY(-JUMPFORCE);
			m_grounded = false; // SetGrounded(false);
			SOMA::PlaySound("jump");
			SetAnimation(STATE_JUMPING, "jump");
		}
		else if (EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			SetAnimation(STATE_IDLING, "idle");
		}
		break;
	}
	// Player movement. X axis first.
	m_velX += m_accelX; // Acceleration
	m_velX *= (m_grounded ? m_drag : 1.0);
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += static_cast<float>(m_velX);
	// Y axis now.
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
	m_dst.y += (float)m_velY;
	
	// Wrapping on screen
	if (m_dst.x < -m_dst.w) {
		m_dst.x = kWidth;
	}
	else if (m_dst.x > kWidth) {
		m_dst.x = -m_dst.w;
	}


	m_accelX = m_accelY = 0.0; // Resetting accel every frame.
	// Invoke the animation.
	Animate();
}

void PlatformPlayer::Render()
{
	//DEMA::DrawRect(*GetDst(), true, { 255, 0, 0, 255 });

	if (m_facingLeft) {
		SDL_RenderCopyExF(REMA::GetRenderer(), TEMA::GetTexture(plrKey),
			GetSrc(), GetDst(), 0, NULL, SDL_FLIP_HORIZONTAL);
	}
	else {
		SDL_RenderCopyExF(REMA::GetRenderer(), TEMA::GetTexture(plrKey),
			GetSrc(), GetDst(), 0, NULL, SDL_FLIP_NONE);
	}
	
}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX() { m_velX = 0.0; }

void PlatformPlayer::StopY() { m_velY = 0.0; }

void PlatformPlayer::SetAccelX(double accel) { m_accelX = accel; }

void PlatformPlayer::SetAccelY(double accel) { m_accelY = accel; }

bool PlatformPlayer::IsGrounded() { return m_grounded; }

void PlatformPlayer::SetGrounded(bool grounded) { m_grounded = grounded; }

double PlatformPlayer::GetVelX() { return m_velX; }

double PlatformPlayer::GetVelY() { return m_velY; }

void PlatformPlayer::SetX(float x) { m_dst.x = x; }

void PlatformPlayer::SetY(float y) { m_dst.y = y; }