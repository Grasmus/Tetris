#include "Button.h"

namespace GameNamespace 
{
	Button::Button(
		POINT position,
		int height,
		int width,
		SDL_Renderer* renderer,
		const char* text,
		TTF_Font* font,
		SDL_Color color)
	{
		this->position = position;
		this->height = height;
		this->width = width;

		buttonRect = {
			position.x,
			position.y,
			width,
			height
		};

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

		SDL_RenderCopy(renderer, message, NULL, &buttonRect);
		SDL_FreeSurface(surface);
	}

	Button::~Button()
	{
		SDL_DestroyTexture(message);
	}

	void Button::RenderButton(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderFillRect(renderer, &buttonRect);
		SDL_RenderCopy(renderer, message, NULL, &buttonRect);
	}

	bool Button::PressButton(POINT pressPoint)
	{
		if (pressPoint.x >= position.x && pressPoint.x <= position.x + width
			&&
			pressPoint.y >= position.y && pressPoint.y <= position.y + height)
		{
			return true;
		}

		return false;
	}
}