#include "StartButton.h"

#include "Engine.h"

void CStartButton::Render()
{
	SDL_Rect src_rect{};
	SDL_Rect dst_rect{};

	int texture_width;
	int texture_height;

	SDL_QueryTexture(s_buttonTextures[START], nullptr, nullptr, &texture_width, &texture_height);

	// Grabbing values from the QueryTexture
	src_rect.x = dst_rect.w = texture_width;
	src_rect.y = dst_rect.h = texture_height;

	// Centering Button
	const int x_offset = static_cast<int>(texture_width * 0.5);
	const int y_offset = static_cast<int>(texture_height * 0.5);


	dst_rect.x = position.x - x_offset;
	dst_rect.y = position.y - y_offset;

	// For when button is pressed
	SDL_SetTextureAlphaMod(s_buttonTextures[START], m_alpha);
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), s_buttonTextures[START], &src_rect, &dst_rect, 0, nullptr, SDL_FLIP_NONE);

}

void CStartButton::OnEnter()
{
}

void CStartButton::OnExit()
{
}

void CStartButton::Execute()
{
}
