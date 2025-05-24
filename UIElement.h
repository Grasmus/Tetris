#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "GameExceptions.h"

namespace GameNamespace
{
	class UIElement
	{
	protected:

		SDL_Rect rect{};

	public:

		virtual void Render(SDL_Renderer* renderer) = 0;
		virtual bool HandleMouseLeftClick(SDL_Point pressPoint) = 0;
		virtual void HandleTextInput(const char* textInput) = 0;
		virtual void HandleKeyDown(SDL_Keycode keyCode) = 0;

		SDL_Rect GetRect() const {
			return rect;
		};

		SDL_Rect CalcTextDimensions(TTF_Font* font, const char* text)
		{
			int textWidth{}, textHeight{};

			if (TTF_SizeText(font, text, &textWidth, &textHeight))
			{
				throw TTFSizeTextException();
			}

			return { 0, 0, textWidth, textHeight };
		}
	};
}
