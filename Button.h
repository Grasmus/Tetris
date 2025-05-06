#pragma once
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include "GameExceptions.h"
#include "UIElement.h"

namespace GameNamespace
{
	class Button : public UIElement
	{
	public:
		Button(
			SDL_Rect rect,
			SDL_Renderer* renderer,
			const char* text,
			TTF_Font* font,
			SDL_Color color,
			void (*function)()
		);

		~Button();

		void Render(SDL_Renderer* renderer);
		void HandleMouseLeftClick(SDL_Point pressPoint);
		void HandleTextInput(const char* textInput) {};
		void HandleKeyDown(SDL_Keycode keyCode) {};

	private:
		SDL_Texture* message{};

		void (*function)();
	};
}
