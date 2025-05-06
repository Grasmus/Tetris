#pragma once
#include <SDL.h>

class UIElement
{
protected:

	SDL_Rect rect{};

public:

	virtual void Render(SDL_Renderer* renderer) = 0;
	virtual void HandleMouseLeftClick(SDL_Point pressPoint) = 0;
	virtual void HandleTextInput(const char* textInput) = 0;
	virtual void HandleKeyDown(SDL_Keycode keyCode) = 0;

	SDL_Rect GetRect() const {
		return rect;
	};
};
