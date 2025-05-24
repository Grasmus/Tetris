#pragma once
#include <vector>
#include <SDL_ttf.h>
#include "DatabaseManager.h"
#include "UIElement.h"
#include <memory>
#include "TextView.h"
#include "LeaderboardRow.h"

namespace GameNamespace
{
	class Leaderboard : public UIElement
	{
	public: 
		Leaderboard(
			SDL_Rect rect,
			SDL_Renderer* renderer,
			int padding,
			std::vector<ScoreEntry> topScores,
			TTF_Font* font,
			SDL_Color textColor,
			SDL_Color backgroundColor);
		~Leaderboard();

		void Render(SDL_Renderer* renderer);
		bool HandleMouseLeftClick(SDL_Point pressPoint) { return false; };
		void HandleTextInput(const char* textInput) {};
		void HandleKeyDown(SDL_Keycode keyCode) {};

		void UpdateTopScores(std::vector<ScoreEntry> topScores);

	private:
		TTF_Font* textFont{};
		SDL_Color textColor{};
		SDL_Renderer* renderer{};
		SDL_Color backgroundColor{};
		int padding{};

		std::vector<ScoreEntry> topScores{};
		std::unique_ptr<TextView> headerTextView{};
		std::vector< std::unique_ptr<LeaderboardRow>> rows{};

		SDL_Rect headerRect{}, rowRect{};

		void CreateHeaderTextView();
		void CreateFirstRow();
		void CreateScoreRows();
	};
}
