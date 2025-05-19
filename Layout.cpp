#include "Layout.h"

namespace GameNamespace
{
	Layout::Layout(int width, int height): 
		windowWidth(width), 
		windowHeight(height)
	{
		CalculateParameters();
	}

	Layout::~Layout() {}

	int Layout::GetWindowWidth() const
	{
		return windowWidth;
	}

	int Layout::GetWindowHeight() const
	{
		return windowHeight;
	}

	int Layout::GetBlockSize() const
	{
		return blockSize;
	}

	int Layout::GetPieceInitialShiftX() const
	{
		return pieceInitialShiftX;
	}

	int Layout::GetScoreYOffset() const
	{
		return scoreYOffset;
	}

	int Layout::GetNextPiecePositionY() const
	{
		return nextPiecePositionY;
	}

	SDL_Rect Layout::GetBoardRect() const
	{
		return boardRect;
	}

	SDL_Rect Layout::GetDrawBoardRect() const
	{
		return drawBoardRect;
	}

	SDL_Rect Layout::GetInfoBlockRect() const
	{
		return infoBlockRect;
	}

	SDL_Rect Layout::GetBackgroundRect() const
	{
		return backgroundRect;
	}

	void Layout::CalculateParameters()
	{
		blockSize = windowWidth / BLOCK_SIZE_SCALER;
		pieceInitialShiftX = blockSize * 4;

		int boardWidth{ BOARD_WIDTH_IN_BLOCKS * blockSize };
		int boardHeight{ BOARD_HEIGHT_IN_BLOCKS * blockSize };

		boardRect = SDL_Rect
		{
			(windowWidth - boardWidth) / 2,
			(windowHeight - boardHeight) / 2,
			boardWidth,
			boardHeight
		};

		drawBoardRect = SDL_Rect
		{
			boardRect.x + blockSize,
			boardRect.y + 2 * blockSize,
			boardRect.w - 2 * blockSize,
			boardRect.h - 3 * blockSize
		};

		infoBlockRect = SDL_Rect
		{
			boardRect.x + boardRect.w,
			boardRect.y + 2 * blockSize,
			INFO_BLOCK_WIDTH_IN_BLOCKS * blockSize,
			INFO_BLOCK_HEIGHT_IN_BLOCKS * blockSize
		};

		scoreYOffset = 11 * infoBlockRect.h / 60;
		nextPiecePositionY = infoBlockRect.y + 8 * blockSize;

		int backgroundRectHeight{ BACKGROUND_RECT_HEIGHT_IN_BLOCKS * blockSize };

		backgroundRect = SDL_Rect
		{
			0,
			(windowHeight - backgroundRectHeight) / 2,
			windowWidth,
			backgroundRectHeight
		};
	}

	void Layout::CalculateParameters(int width, int height)
	{
		windowWidth = width;
		windowHeight = height;

		CalculateParameters();
	}
}
