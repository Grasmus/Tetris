#pragma once

#include <SDL_ttf.h>
#include <string>
#include "UIElement.h"
#include "GameExceptions.h"

namespace GameNamespace
{
	class TextView : public UIElement
	{
	public:
		TextView(
			SDL_Rect rect,
			TTF_Font* font,
			SDL_Renderer* renderer,
			SDL_Color textColor,
			const char* text);

		~TextView();

		void Render(SDL_Renderer* renderer);
		bool HandleMouseLeftClick(SDL_Point pressPoint) { return false; };
		void HandleTextInput(const char* textInput) {};
		void HandleKeyDown(SDL_Keycode keyCode) {};

		void SetText(SDL_Rect rect, const char* text, SDL_Renderer* renderer);

	private:
		TTF_Font* font{};
		SDL_Color textColor{};
		std::string text{};
		SDL_Texture* message{};
	};
}
