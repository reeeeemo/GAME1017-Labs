#include "Button.h"
#include "Engine.h"
#include "StartButton.h"
#include "PauseButton.h"
#include "EndButton.h"


std::array<CButton*, CButton::NUM_BUTTONS> CButton::s_buttons;
std::array<SDL_Texture*, CButton::NUM_BUTTONS> CButton::s_buttonTextures;
CButton* s_currentButton = nullptr;


void CButton::Init()
{
	// Loads all components into the 2 arrays for textures and the actual buttons.
	s_buttons[START] = new CStartButton;
	s_buttons[PAUSE] = new CPauseButton;
	s_buttons[END] = new CEndButton;
	s_buttonTextures[START] = IMG_LoadTexture(Engine::Instance().GetRenderer(), "");
	s_buttonTextures[PAUSE] = IMG_LoadTexture(Engine::Instance().GetRenderer(), "");
	s_buttonTextures[END] = IMG_LoadTexture(Engine::Instance().GetRenderer(), "");

	// Start button is enabled by default.
	s_currentButton = s_buttons[START];
}

void CButton::Update()
{
	if (s_currentButton->isActive)
	{
		s_currentButton->m_alpha = 127;
	} else
	{
		s_currentButton->m_alpha = 255;
	}
}

void CButton::Render()
{
	s_currentButton->Render();
}

void CButton::SetPosition(int x, int y)
{
	s_currentButton->position = { x, y };
}

