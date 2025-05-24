#include "LeaderboardRow.h"

namespace GameNamespace
{
	LeaderboardRow::LeaderboardRow(
		SDL_Renderer* renderer,
		SDL_Point position,
		int width,
		TTF_Font* font,
		SDL_Color textColor,
		const char* numberColumnText,
		const char* usernameColumnText,
		const char* scoreColumnText
	) : renderer(renderer),
		position(position),
		width(width),
		textFont(font),
		textColor(textColor)
	{
		if (font == nullptr)
		{
			throw FontNullReference();
		}

		this->textFont = font;

		CreateNumberColumnTitleTextView(numberColumnText);
		CreateScoreColumnTitleTextView(scoreColumnText);
		CreateUsernameColumnTitleTextView(usernameColumnText);
	}

	LeaderboardRow::~LeaderboardRow() {}

	void LeaderboardRow::Render(SDL_Renderer* renderer)
	{
		numberColumnTitleTextView->Render(renderer);
		usernameColumnTitleTextView->Render(renderer);
		scoreColumnTitleTextView->Render(renderer);
	}

	SDL_Point LeaderboardRow::GetPosition() const
	{
		return position;
	}

	int LeaderboardRow::GetHeight() const
	{
		return height;
	}

	void LeaderboardRow::CreateNumberColumnTitleTextView(const char* numberColumnText)
	{
		numberColumnRect = CalcTextDimensions(textFont, numberColumnText);

		height = numberColumnRect.h > height ? numberColumnRect.h : height;

		numberColumnRect.x = position.x;
		numberColumnRect.y = position.y;

		numberColumnTitleTextView = std::make_unique<TextView>(
			numberColumnRect,
			textFont,
			renderer,
			textColor,
			numberColumnText
		);
	}

	void LeaderboardRow::CreateUsernameColumnTitleTextView(const char* usernameColumnText)
	{
		usernameColumnRect = CalcTextDimensions(textFont, usernameColumnText);

		height = usernameColumnRect.h > height ? usernameColumnRect.h : height;

		usernameColumnRect.x =
			(numberColumnRect.x + numberColumnRect.w + scoreColumnRect.x - usernameColumnRect.w) / 2;
		usernameColumnRect.y = numberColumnRect.y;

		usernameColumnTitleTextView = std::make_unique<TextView>(
			usernameColumnRect,
			textFont,
			renderer,
			textColor,
			usernameColumnText
		);
	}

	void LeaderboardRow::CreateScoreColumnTitleTextView(const char* scoreColumnText)
	{
		scoreColumnRect = CalcTextDimensions(textFont, scoreColumnText);

		height = scoreColumnRect.h > height ? scoreColumnRect.h : height;

		scoreColumnRect.x = position.x + width - scoreColumnRect.w;
		scoreColumnRect.y = numberColumnRect.y;

		scoreColumnTitleTextView = std::make_unique<TextView>(
			scoreColumnRect,
			textFont,
			renderer,
			textColor,
			scoreColumnText
		);
	}
}
