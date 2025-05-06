#include "Button.h"

namespace GameNamespace 
{
	Button::Button(
		SDL_Rect rect,
		SDL_Renderer* renderer,
		const char* text,
		TTF_Font* font,
		SDL_Color color,
		void (*function)()
	): function(function)
	{
		this->rect = rect;

		SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);

		if (surface == NULL)
		{
			throw SurfaceNullReference();
		}

		message = SDL_CreateTextureFromSurface(renderer, surface);

		if (message == NULL)
		{
			throw MessageNullReference();
		}

		SDL_RenderCopy(renderer, message, NULL, &rect);
		SDL_FreeSurface(surface);
	}

	Button::~Button()
	{
		SDL_DestroyTexture(message);
	}

	void Button::Render(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderCopy(renderer, message, NULL, &rect);
	}

	void Button::HandleMouseLeftClick(SDL_Point pressPoint)
	{
		if (pressPoint.x >= rect.x && pressPoint.x <= rect.x + rect.w
			&&
			pressPoint.y >= rect.y && pressPoint.y <= rect.y + rect.h)
		{
			function();
		}
	}
}