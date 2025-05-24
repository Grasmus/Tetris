#pragma once

#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "ResourceHandler.h"
#include "TextInput.h"
#include "Strings.h"
#include "SettingsElement.h"
#include "DatabaseManager.h"
#include "Leaderboard.h"

namespace GameNamespace
{
	enum class UIDestination
	{
		Menu,
		Settings,
		Leaderboard,
		EnterUsername
	};

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
		void ResetFigurePosition();
		void UpdateTopScores();

		static void EnterUsername();
		static void StartGame();
		static void EnterSettings();
		static void SaveSettings();
		static void EnterLeaderboard();
		static void EnterMenu();
		static void UpdateUsernameFromTextInput(std::string& text);

		static Game* Init();

		static Game* game;

	private:
		SDL_Renderer* renderer{};
		SDL_Window* window{};
		std::unique_ptr<ResourceHandler> resourceHandler{};
		std::unique_ptr<Settings> settings{};
		std::unique_ptr<Layout> layout{};
		std::unique_ptr<Leaderboard> leaderboard{};
		std::vector<std::vector<int>> board{};
		FigureKind currentFigure{};
		FigureKind nextFigure{};
		size_t rotation{};
		size_t nextRotation{};

		SDL_Point currentFigurePosition{};

		std::map<UIDestination, std::vector<std::unique_ptr<UIElement>>> UIElements{};

		GameState gameState{ GameState::Menu };

		PieceMovement pieceMovement{ PieceMovement::None };

		int currentFrame{};
		int score{};

		std::unique_ptr<DatabaseManager> dbManager{};
		std::string username{};
		std::string gameStartTime{};

		bool isScoreSaved = false;

		std::vector<ScoreEntry> topScores;

		SDL_Rect playButtonRect{};

		void HandleGameEvent(SDL_Event event, UIDestination destination, GameState exitState);
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

		void CreateLayout();
		void CreateUI();
		void ChangeWindowSize();

		void AddButton(
			SDL_Rect buttonRect,
			Font font,
			Color color,
			const char* text,
			void (*function)(),
			UIDestination destination
		);

		void AddTextInput(
			SDL_Rect textInputRect,
			Font font,
			Color textColor,
			Color caretteColor,
			int textMaxLenght,
			void (*onTextInput)(std::string&),
			UIDestination destination
		);

		void AddTextView(
			SDL_Rect textViewRect,
			const char* text,
			Font font,
			Color textColor,
			UIDestination destination
		);

		void AddSettingsElement(
			SDL_Point position,
			int width,
			SettingsType settingType,
			const char* settingName,
			std::vector<std::string> values,
			int currentValueIndex,
			Font font,
			Color textColor
		);

		void AddLeaderboard(
			SDL_Rect rect,
			int padding,
			std::vector<ScoreEntry> topScores,
			Font font,
			Color textColor,
			Color backgroundColor
		);

		void RenderElements(UIDestination destination);

		std::string GetCurrentTimeString();

		void HandleMouseLeftClick(int mouseCoordinateX, int mouseCoordinateY, UIDestination destination);
		void HandleTextInput(const char* tex, UIDestination destinationt);
		void HandleKeyDown(SDL_Keycode keyCode, UIDestination destination);
	};
}
