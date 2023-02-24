#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <SDL_rect.h>
#include <string>
#include <array>
#include <SDL_render.h>
#include "StateManager.h"

class CButton
{
public:
	CButton() = default;
	virtual ~CButton() = default;


	static void Init();
	static void Update();
	static void Render();
	static void Exit();

	static void SetPosition(int x, int y);
	static void SetEnabled(bool enabled);

	static bool IsMouseOver();

protected:

	virtual void OnEnter() = 0;
	virtual void Execute() = 0;

	enum EButtons
	{
		START,
		PAUSE,
		END,
		NUM_BUTTONS
	};

	std::string m_key;
	Uint8 m_alpha;
	SDL_Rect m_dst; // This could be another component (button is entity), but this will do for now.
	bool isActive = false;
	bool isEnabled = false;

	static std::array<CButton*, NUM_BUTTONS> s_buttons;
	static std::array<SDL_Texture*, NUM_BUTTONS> s_buttonTextures;
	static CButton* s_currentButton;
	static SDL_Texture* s_currentTexture;
};	



#endif // !__BUTTON_H__