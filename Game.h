#pragma once

#include <Windows.h>
#include "Constants.h"
#include <SDL.h>
#include <SDL_main.h>
#include <vector>

namespace GameNamespace
{
	class Game
	{
	public:
		Game();
		~Game();

		void HandleEvents();
		void Render();
		void Update();
		bool Running();
		void AddFrame();
		int GetFrameDelay();

	private:
		SDL_Renderer* renderer{};
		SDL_Window* window{};
		TTF_Font* gameOverFont{};
		TTF_Font* sceneFont{};
		std::vector<std::vector<int>> board{};
		Figure currentFigure{};
		Figure nextFigure{};
		size_t rotation{};
		size_t nextRotation{};
		POINT currentFigurePosition
		{
			BOARD_POSITION_X + PIECE_INITIAL_SHIFT_X,
			BOARD_POSITION_Y - PIECE_INITIAL_SHIFT_Y
		};
		POINT boardPosition
		{
			BOARD_POSITION_X,
			BOARD_POSITION_Y
		};
		bool isRunning{ false };
		bool isGameOver{ false };
		bool leftMove{ false };
		bool rightMove{ false };
		bool rotationMove{ false };
		bool speedUpMove{ false };
		bool startAgain{ false };
		int currentFrame{};
		int score{};

		std::vector<std::vector<int>> InitializeBoard();
		bool CheckIsPieceCanMove();
		bool CheckIsPieceCanMove(Direction direction);
		bool CheckIsPieceCanRotate();
		void CheckIsGameOver();
		int CalculateNextRotation();
		void DeleteLines();
		void DropUpperBlocks(int yIndex);
		void SaveCurrentPiece();
		void DrawFigure();
		void DrawFigure(Figure figure, size_t rotation, int x, int y);
		void DrawBoard();
		void DrawScene();
		void DrawBlock(POINT point, Color color);
		void SetColor(Color color);
		void CreateMessage(
			Font fontKind,
			const char* text,
			SDL_Color color,
			SDL_Rect messageRectangle);
		TTF_Font* GetFont(Font font);
		void PrintGameOver();
		void AddScore();
	};
}
