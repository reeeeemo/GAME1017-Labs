#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <SDL_rect.h>
#include <string>
#include <array>
#include <SDL_render.h>

class CButton
{
public:
	CButton() = default;


	void Init();
	void Update();
	virtual void Render();

	void SetPosition(int x, int y);

protected:

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void Execute() = 0;

	enum EButtons
	{
		START,
		PAUSE,
		END,
		NUM_BUTTONS
	};

	static std::array<CButton*, NUM_BUTTONS> s_buttons;
	static std::array<SDL_Texture*, NUM_BUTTONS> s_buttonTextures;
	static CButton* s_currentButton;

	SDL_Point position{}; // This could be another component (button is entity), but this will do for now.
	std::string m_key;
	Uint8 m_alpha;
	SDL_FRect m_dst;
	bool isActive = false;
};	



#endif // !__BUTTON_H__