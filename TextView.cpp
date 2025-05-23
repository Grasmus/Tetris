#include "TextView.h"

namespace GameNamespace
{
	TextView::TextView(
		SDL_Rect rect,
		TTF_Font* font,
		SDL_Renderer* renderer,
		SDL_Color textColor,
		const char* text
	):
		textColor(textColor)
	{
		this->rect = rect;

		if (font == NULL)
		{
			throw FontNullReference();
		}

		this->font = font;

		SetText(rect, text, renderer);
	}

	TextView::~TextView() {}

	void TextView::Render(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderCopy(renderer, message, NULL, &rect);
	}

	void TextView::SetText(SDL_Rect rect, const char* text, SDL_Renderer* renderer)
	{
		this->rect = rect;

		SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);

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
}
