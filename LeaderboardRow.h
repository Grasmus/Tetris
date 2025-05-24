#pragma once
#include "UIElement.h"
#include <SDL_ttf.h>
#include <memory>
#include "TextView.h"

namespace GameNamespace
{
	class LeaderboardRow : public UIElement
	{
	public:
		LeaderboardRow(
			SDL_Renderer* renderer,
			SDL_Point position,
			int width,
			TTF_Font* font,
			SDL_Color textColor,
			const char* numberColumnText,
			const char* usernameColumnText,
			const char* scoreColumnText
		);
		~LeaderboardRow();

		void Render(SDL_Renderer* renderer);
		bool HandleMouseLeftClick(SDL_Point pressPoint) { return false; };
		void HandleTextInput(const char* textInput) {};
		void HandleKeyDown(SDL_Keycode keyCode) {};

		SDL_Point GetPosition() const;
		int GetHeight() const;

	private:
		TTF_Font* textFont{};
		SDL_Color textColor{};
		SDL_Renderer* renderer{};
		SDL_Point position{};
		int width{};
		int height{};

		std::unique_ptr<TextView> numberColumnTitleTextView{};
		std::unique_ptr<TextView> usernameColumnTitleTextView{};
		std::unique_ptr<TextView> scoreColumnTitleTextView{};

		SDL_Rect numberColumnRect{};
		SDL_Rect usernameColumnRect{};
		SDL_Rect scoreColumnRect{};

		void CreateNumberColumnTitleTextView(const char* numberColumnText);
		void CreateUsernameColumnTitleTextView(const char* usernameColumnText);
		void CreateScoreColumnTitleTextView(const char* scoreColumnText);
	};
}
