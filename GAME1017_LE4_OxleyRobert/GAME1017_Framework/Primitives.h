#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__
#include "GameObject.h"
#include <string>

class Image : public Sprite
{
public:
	Image(SDL_Rect src, SDL_FRect dst , const char* key);
	void Update() {};
	void Render();
private:
	std::string m_key;
};

class Line : public GameObject
{
public:
	Line(SDL_FPoint start, SDL_FPoint end, SDL_Color col = { 255, 255, 255, 255 });
	void Update() {};
	void Render();
private:
	SDL_FPoint m_start, m_end;
	SDL_Color m_color;
};

class Rectangle : public GameObject
{
public:
	Rectangle(SDL_FRect dst, bool fill, SDL_Color col = { 255, 255, 255, 255 });
	void Update() {};
	void Render();
private:
	bool m_fill;
	SDL_Color m_color;
};

#endif