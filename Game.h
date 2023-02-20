#pragma once

#include <Windows.h>
#include "Constants.h"
#include <SDL.h>
#include <SDL_main.h>
#include <vector>
#include "Button.h"
#include <memory>

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
		bool IsRunning();
		int GetFrameDelay();

	private:
		SDL_Renderer* renderer{};
		SDL_Window* window{};
		TTF_Font* gameOverFont{};
		TTF_Font* sceneFont{};
		SDL_Texture* blockTexture{};
		SDL_Texture* backgroundTexture{};
		SDL_Texture* boardTexture{};
		SDL_Texture* infoBlockTexture{};
		std::vector<std::vector<int>> board{};
		FigureKind currentFigure{};
		FigureKind nextFigure{};
		size_t rotation{};
		size_t nextRotation{};
		POINT currentFigurePosition
		{
			BOARD_POSITION_X + PIECE_INITIAL_SHIFT_X,
			BOARD_POSITION_Y
		};
		POINT boardPosition
		{
			BOARD_POSITION_X,
			BOARD_POSITION_Y
		};
		std::unique_ptr<Button> menuButton{};

		GameState gameState{ GameState::MenuMode };

		PieceMovement pieceMovement{ PieceMovement::None };

		int currentFrame{};
		int score{};

		void HandleMainMenuEvent(SDL_Event event);
		void HandleGameEvent(SDL_Event event);
		void HandleGamePausedEvent(SDL_Event event);
		void HandleGameOverEvent(SDL_Event event);

		void MovePiece();
		void InitializeGame();
		void GoToNextPiece();

		std::vector<std::vector<int>> InitializeBoard();
		bool CheckIsPieceCanMove();
		bool CheckIsPieceCanMove(Direction direction);
		PieceRotation CheckIsPieceCanRotate();
		void CheckIsGameOver();
		int CalculateNextRotation();
		void AddFrame();
		void DeleteLines();
		void DropUpperBlocks(int yIndex);
		void SaveCurrentPiece();
		void DrawFigure();
		void DrawFigure(FigureKind figure, size_t rotation, int x, int y);
		void DrawBoard();
		void DrawScene();
		void DrawBlock(POINT point, Color color);
		void DrawBlock(POINT point, SDL_Texture* texture);
		void SetColor(Color color);
		void CreateMessage(
			Font fontKind,
			const char* text,
			SDL_Color color,
			SDL_Rect messageRectangle);
		TTF_Font* GetFont(Font font);
		void PrintGameOver();
		void AddScore();
		void PrintPauseGame();
		SDL_Texture* LoadTexture(const char* textureFilePath);
	};
}
