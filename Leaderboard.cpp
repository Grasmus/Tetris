#include "Leaderboard.h"

namespace GameNamespace
{
	const char* LEADERBOARD_HEADER_TEXT{ "Top Scores" };
	const char* LEADERBOARD_NUMBER_COLUMN_TITLE{ "#" };
	const char* LEADERBOARD_USERNAME_COLUMN_TITLE{ "Name" };
	const char* LEADERBOARD_SCORE_COLUMN_TITLE{ "Score" };

	Leaderboard::Leaderboard(
		SDL_Rect rect,
		SDL_Renderer* renderer,
		int padding,
		std::vector<ScoreEntry> topScores,
		TTF_Font* font,
		SDL_Color textColor,
		SDL_Color backgroundColor
	) : padding(padding),
		renderer(renderer),
		textColor(textColor),
		backgroundColor(backgroundColor),
		topScores(topScores)
	{
		this->rect = rect;

		if (font == NULL)
		{
			throw FontNullReference();
		}

		this->textFont = font;

		CreateHeaderTextView();
		CreateFirstRow();
		CreateScoreRows();
	}

	Leaderboard::~Leaderboard() {}

	void Leaderboard::Render(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(
			renderer,
			backgroundColor.r,
			backgroundColor.g,
			backgroundColor.b,
			backgroundColor.a
		);
		SDL_RenderFillRect(renderer, &rect);

		for (auto& row : rows)
		{
			row->Render(renderer);
		}

		headerTextView->Render(renderer);
	}

	void Leaderboard::UpdateTopScores(std::vector<ScoreEntry> topScores)
	{
		this->topScores = topScores;

		rows.clear();

		CreateFirstRow();
		CreateScoreRows();
	}

	void Leaderboard::CreateHeaderTextView()
	{
		headerRect = CalcTextDimensions(textFont, LEADERBOARD_HEADER_TEXT);

		headerRect.x = rect.x + (rect.w - headerRect.w) / 2;
		headerRect.y = rect.y + padding;

		headerTextView = std::make_unique<TextView>(
			headerRect,
			textFont,
			renderer,
			textColor,
			LEADERBOARD_HEADER_TEXT
		);
	}

	void Leaderboard::CreateFirstRow()
	{
		rowRect = CalcTextDimensions(textFont, LEADERBOARD_NUMBER_COLUMN_TITLE);

		rowRect.x = rect.x + padding;
		rowRect.y = headerRect.y + headerRect.h + rowRect.h / 2;

		rowRect.w = rect.w - 2 * padding;

		SDL_Point firstRowPosition
		{
			rowRect.x,
			rowRect.y
		};

		rows.push_back(
			std::make_unique<LeaderboardRow>
			(
				renderer,
				firstRowPosition,
				rowRect.w,
				textFont,
				textColor,
				LEADERBOARD_NUMBER_COLUMN_TITLE,
				LEADERBOARD_USERNAME_COLUMN_TITLE,
				LEADERBOARD_SCORE_COLUMN_TITLE
			)
		);
	}

	void Leaderboard::CreateScoreRows()
	{
		for (int i{}; i < topScores.size(); i++)
		{
			SDL_Point position{ rows.back()->GetPosition() };

			position.y += (rowRect.h * 3) / 2;

			rows.push_back(
				std::make_unique<LeaderboardRow>
				(
					renderer,
					position,
					rowRect.w,
					textFont,
					textColor,
					std::to_string(i).c_str(),
					topScores[i].username.c_str(),
					std::to_string(topScores[i].score).c_str()
				)
			);
		}

		rect.h =
			rows.back()->GetPosition().y - rect.y + rows.back()->GetHeight() + padding;
	}
}
