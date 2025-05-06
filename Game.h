#pragma once

#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "ResourceHandler.h"
#include "Button.h"
#include "TextInput.h"
#include "Strings.h"

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
		bool IsRunning() const;
		int GetFrameDelay() const;
		
		static void StartGame();
		static Game* Init();

		static Game* game;

	private:
		SDL_Renderer* renderer{};
		SDL_Window* window{};
		std::unique_ptr<ResourceHandler> resourceHandler{};
		std::vector<std::vector<int>> board{};
		FigureKind currentFigure{};
		FigureKind nextFigure{};
		size_t rotation{};
		size_t nextRotation{};

		SDL_Point currentFigurePosition
		{
			BOARD_POSITION_X + PIECE_INITIAL_SHIFT_X,
			BOARD_POSITION_Y
		};

		SDL_Point boardPosition
		{
			BOARD_POSITION_X,
			BOARD_POSITION_Y
		};

		std::vector<std::unique_ptr<UIElement>> UIElements{};

		GameState gameState{ GameState::MenuMode };

		PieceMovement pieceMovement{ PieceMovement::None };

		int currentFrame{};
		int score{};

		void HandleMainMenuEvent(SDL_Event event);
		void HandleGameEvent(SDL_Event event);
		void HandleGamePausedEvent(SDL_Event event);
		void HandleGameOverEvent(SDL_Event event);

		void MovePiece();
		void GoToNextPiece();

		std::vector<std::vector<int>> InitializeBoard();
		bool CheckIsPieceCanMove();
		bool CheckIsPieceCanMove(Direction direction);
		PieceRotation CheckIsPieceCanRotate();
		void CheckIsGameOver();
		int CalculateNextRotation() const;
		void AddFrame();
		void DeleteLines();
		void DropUpperBlocks(int yIndex);
		void SaveCurrentPiece();
		void DrawFigure();
		void DrawFigure(FigureKind figure, size_t rotation, int x, int y);
		void DrawBoard();
		void DrawScene();
		void DrawBlock(SDL_Point position, Color color);
		void DrawBlock(SDL_Point position, SDL_Texture* texture);
		void SetColor(Color color);
		void CreateMessage(
			Font fontKind,
			const char* text,
			Color color,
			int width, 
			int height, 
			SDL_Point position);
		void PrintGameOver();
		void AddScore();
		void PrintPauseGame();
		SDL_Texture* LoadTexture(const char* textureFilePath);

		unsigned CalcRelativeWidth(unsigned width);
		unsigned CalcRelativeHeight(unsigned height);

		SDL_Color GetColor(Color color);
		SDL_Rect CalcTextDimensions(Font fontType, const char* text);

		void CreateUI();

		Button* CreateButton(
			SDL_Rect buttonRect,
			Font font,
			Color color,
			const char* text,
			void (*function)()
		);

		TextInput* CreateTextInput(
			SDL_Rect textInputRect,
			Font font,
			Color textColor,
			Color caretteColor,
			int textMaxLenght
		);

		void AddButton(
			SDL_Rect buttonRect,
			Font font,
			Color color,
			const char* text,
			void (*function)()
		);

		void AddTextInput(
			SDL_Rect textInputRect,
			Font font,
			Color textColor,
			Color caretteColor,
			int textMaxLenght
		);

		void RenderMenu();

		void HandleMenuMouseLeftClick(int mouseCoordinateX, int mouseCoordinateY);
		void HandleMenuTextInput(const char* text);
		void HandleMenuKeyDown(SDL_Keycode keyCode);
	};
}
