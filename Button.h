#pragma once
#include <SDL.h>
#include <SDL_main.h>
#include <Windows.h>
#include "Constants.h"
#include "GameExceptions.h"

namespace GameNamespace
{
	class Button
	{
	public:
		Button(
			POINT point,
			int width,
			int height,
			SDL_Renderer* renderer,
			const char* text,
			TTF_Font* font,
			SDL_Color color,
			void (*function)()
		);

		~Button();
		void Render(SDL_Renderer* renderer);
		void HandleMouseLeftClick(POINT pressPoint);

	private:
		POINT position{};
		int height{};
		int width{};
		SDL_Rect buttonRect{};

		SDL_Texture* message{};

		void (*function)();
	};
}
