#include "Game.h"
#include "GameExceptions.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL_image.h>

namespace GameNamespace
{
	Game::Game()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING))
		{
			throw SDLInitException();
		}

		window = SDL_CreateWindow(
			GAME_WINDOW_NAME,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			0
		);

		if (window == NULL)
		{
			throw WindowCreationException();
		}

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer == NULL)
		{
			throw RenderCreationException();
		}

		if (TTF_Init() == -1)
		{
			throw TTFInitException();
		}

		gameOverFont = TTF_OpenFont(FONT_FILE_PATH, MAIN_FONT_SIZE);
		sceneFont = TTF_OpenFont(FONT_FILE_PATH, SCENE_FONT_SIZE);

		if (gameOverFont == NULL || sceneFont == NULL)
		{
			throw FontNullReference();
		}

		blockTexture = LoadTexture(BLOCK_TEXTURE_FILE_PATH);
		backgroundTexture = LoadTexture(BACKGROUND_TEXTURE_FILE_PATH);
		boardTexture = LoadTexture(BOARD_TEXTURE_FILE_PATH);
		infoBlockTexture = LoadTexture(INFO_BLOCK_TEXTURE_FILE_PATH);

		if (SDL_SetTextureAlphaMod(boardTexture, 100))
		{
			throw SetTextureAlphaModException();
		}

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		srand(time(NULL));

		menuButton = std::make_unique<Button>(
			MENU_BUTTON_POINT,
			BUTTON_HEIGHT, 
			BUTTON_WIDTH, 
			renderer, 
			"Play", 
			sceneFont, 
			BUTTON_FONT_COLOR);
	}

	Game::~Game()
	{
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

	void Game::HandleEvents()
	{
		SDL_Event event{};

		SDL_PollEvent(&event);

		switch (gameState)
		{
		case GameState::Running:
			HandleGameEvent(event);
			break;

		case GameState::Paused:
			HandleGamePausedEvent(event);
			break;

		case GameState::GameOver:
			HandleGameOverEvent(event);
			break;

		case GameState::MenuMode:
			HandleMainMenuEvent(event);
			break;

		default:
			break;
		}
	}

	void Game::Render()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

		switch (gameState)
		{
		case GameState::Running:
			DrawBoard();
			DrawFigure();
			DrawScene();
			break;

		case GameState::Paused:
			DrawBoard();
			DrawFigure();
			DrawScene();
			PrintPauseGame();
			break;

		case GameState::GameOver:
			DrawBoard();
			DrawFigure();
			DrawScene();
			PrintGameOver();
			break;

		case GameState::MenuMode:
			menuButton->RenderButton(renderer);
			break;

		default:
			break;
		}

		SDL_RenderPresent(renderer);
	}

	void Game::Update()
	{
		switch (gameState)
		{
		case GameState::Running:

			MovePiece();

			break;

		default:
			break;
		}

		AddFrame();
	}

	bool Game::IsRunning()
	{
		return gameState != GameState::Inactive;
	}

	void Game::DrawBlock(POINT point, Color color)
	{
		SDL_Rect rect
		{
			point.x,
			point.y,
			BLOCK_SIZE,
			BLOCK_SIZE
		};

		SetColor(color);
		SDL_RenderFillRect(renderer, &rect);
	}

	void Game::DrawBlock(POINT point, SDL_Texture* texture)
	{
		SDL_Rect rect
		{
			point.x,
			point.y,
			BLOCK_SIZE,
			BLOCK_SIZE
		};

		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}

	void Game::SetColor(Color color)
	{
		switch (color)
		{
		case Color::red:
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			break;

		case Color::green:
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			break;

		case Color::black:
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			break;

		case Color::blue:
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			break;

		case Color::white:
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			break;

		case Color::transparentBlack:
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
			break;

		default:
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
	}

	int Game::GetFrameDelay()
	{
		return FRAME_DELAY;
	}

	void Game::DrawFigure()
	{
		int primalPosition{ currentFigurePosition.x };
		POINT piecePosition{ currentFigurePosition };

		for (size_t i{}; i < Figures[static_cast<int>(currentFigure)][rotation].size(); i++)
		{
			for (size_t j{}; j < Figures[static_cast<int>(currentFigure)][rotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(currentFigure)][rotation][i][j] == 0)
				{
					piecePosition.x += BLOCK_SIZE;
				}
				else
				{
					DrawBlock(piecePosition, blockTexture);
					piecePosition.x += BLOCK_SIZE;
				}
			}

			piecePosition.y += BLOCK_SIZE;
			piecePosition.x = primalPosition;
		}
	}

	void Game::DrawFigure(FigureKind figure, size_t rotation, int x, int y)
	{
		int primalXPosition{ x };
		POINT piecePosition{ x, y };

		for (size_t i{}; i < Figures[static_cast<int>(figure)][rotation].size(); i++)
		{
			for (size_t j{}; j < Figures[static_cast<int>(figure)][rotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(figure)][rotation][i][j] == 0)
				{
					piecePosition.x += BLOCK_SIZE;
				}
				else
				{
					DrawBlock(piecePosition, blockTexture);
					piecePosition.x += BLOCK_SIZE;
				}
			}

			piecePosition.y += BLOCK_SIZE;
			piecePosition.x = primalXPosition;
		}
	}

	void Game::DrawBoard()
	{
		SDL_RenderCopy(renderer, boardTexture, NULL, &BOARD_RECT);
		int primalXPosition{ boardPosition.x };
		POINT blockPosition{ boardPosition };

		for (int i{}; i < BOARD_HEIGHT_IN_BLOCKS; i++)
		{
			for (int j{}; j < BOARD_WIDTH_IN_BLOCKS; j++)
			{
				switch (board[i][j])
				{
				case 3:
					break;

				case 2:
					DrawBlock(blockPosition, blockTexture);
					break;

				default:
					break;
				}

				blockPosition.x += BLOCK_SIZE;
			}

			blockPosition.x = primalXPosition;
			blockPosition.y += BLOCK_SIZE;
		}
	}

	void Game::HandleMainMenuEvent(SDL_Event event)
	{
		int mouseCoordinateX{}, mouseCoordinateY{};

		switch (event.type)
		{
		case SDL_QUIT:
			gameState = GameState::Inactive;
			break;

		case SDL_MOUSEBUTTONDOWN:

			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:

				SDL_GetMouseState(&mouseCoordinateX, &mouseCoordinateY);

				if (menuButton->PressButton({ mouseCoordinateX, mouseCoordinateY }))
				{
					InitializeGame();
				}
				break;

			default:
				break;
			}

			break;

		default:
			break;
		}
	}

	void Game::HandleGameEvent(SDL_Event event)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameState = GameState::Inactive;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
				pieceMovement = PieceMovement::Left;
				break;

			case SDLK_LEFT:
				pieceMovement = PieceMovement::Left;
				break;

			case SDLK_d:
				pieceMovement = PieceMovement::Right;
				break;

			case SDLK_RIGHT:
				pieceMovement = PieceMovement::Right;
				break;

			case SDLK_SPACE:
				pieceMovement = PieceMovement::Rotation;
				break;

			case SDLK_s:
				pieceMovement = PieceMovement::SpeedUp;
				break;

			case SDLK_DOWN:
				pieceMovement = PieceMovement::SpeedUp;
				break;

			case SDLK_ESCAPE:
				gameState = GameState::Paused;
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}

	void Game::HandleGamePausedEvent(SDL_Event event)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameState = GameState::Inactive;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				gameState = GameState::Running;
				break;

			case SDLK_ESCAPE:
				gameState = GameState::MenuMode;
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}

	void Game::HandleGameOverEvent(SDL_Event event)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameState = GameState::Inactive;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				InitializeGame();
				break;

			case SDLK_ESCAPE:
				gameState = GameState::MenuMode;
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}

	void Game::MovePiece()
	{
		PieceRotation pieceRotation{};

		switch (pieceMovement)
		{
		case PieceMovement::Left:

			if (CheckIsPieceCanMove(Direction::Left))
			{
				currentFigurePosition.x -= BLOCK_SIZE;
			}
			break;

		case PieceMovement::Right:

			if (CheckIsPieceCanMove(Direction::Right))
			{
				currentFigurePosition.x += BLOCK_SIZE;
			}
			break;

		case PieceMovement::Rotation:

			pieceRotation = CheckIsPieceCanRotate();

			if (pieceRotation.pieceCanRotate)
			{
				currentFigurePosition.x += pieceRotation.pieceShift * BLOCK_SIZE;
				rotation = pieceRotation.nextRotation;
			}
			break;

		case PieceMovement::SpeedUp:

			if (CheckIsPieceCanMove())
			{
				currentFigurePosition.y += BLOCK_SIZE;
			}
			else
			{
				GoToNextPiece();
			}

			pieceMovement = PieceMovement::None;

			break;

		case PieceMovement::None:

			if (CheckIsPieceCanMove())
			{
				if (currentFrame == FPS)
				{
					currentFigurePosition.y += BLOCK_SIZE;
				}
			}
			else
			{
				GoToNextPiece();
			}

			break;

		default:
			break;
		}

		pieceMovement = PieceMovement::None;
	}

	void Game::InitializeGame()
	{
		board = InitializeBoard();
		currentFigure = (FigureKind)(rand() % PIECE_KINDS);
		nextFigure = (FigureKind)(rand() % PIECE_KINDS);
		rotation = rand() % PIECE_ROTATIONS;
		nextRotation = rand() % PIECE_ROTATIONS;

		currentFigurePosition = {
				BOARD_POSITION_X + PIECE_INITIAL_SHIFT_X,
				BOARD_POSITION_Y
		};

		score = 0;

		gameState = GameState::Running;
	}

	void Game::GoToNextPiece()
	{
		if (currentFrame == FPS)
		{
			SaveCurrentPiece();

			currentFigure = nextFigure;
			nextFigure = (FigureKind)(rand() % PIECE_KINDS);
			rotation = nextRotation;
			nextRotation = rand() % PIECE_ROTATIONS;
			currentFigurePosition = {
				BOARD_POSITION_X + PIECE_INITIAL_SHIFT_X,
				BOARD_POSITION_Y
			};

			DeleteLines();
			CheckIsGameOver();
		}
	}

	std::vector<std::vector<int>> Game::InitializeBoard()
	{
		std::vector<std::vector<int>> board(
			BOARD_HEIGHT_IN_BLOCKS, std::vector<int>(BOARD_WIDTH_IN_BLOCKS));

		for (int i{}; i < BOARD_HEIGHT_IN_BLOCKS; i++)
		{
			for (int j{}; j < BOARD_WIDTH_IN_BLOCKS; j++)
			{
				if (j == 0 || i == BOARD_HEIGHT_IN_BLOCKS - 1 || j == BOARD_WIDTH_IN_BLOCKS - 1)
				{
					board[i][j] = 3;
				}
				else
				{
					board[i][j] = 0;
				}
			}
		}

		return board;
	}

	bool Game::CheckIsPieceCanMove()
	{
		int xIndex = (currentFigurePosition.x - BOARD_POSITION_X) / BLOCK_SIZE;
		int yIndex = (currentFigurePosition.y - BOARD_POSITION_Y) / BLOCK_SIZE + 1;

		for (int i{}; i < Figures[static_cast<int>(currentFigure)][rotation].size(); i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < Figures[static_cast<int>(currentFigure)][rotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(currentFigure)][rotation][i][j] != 0)
				{
					if (board[yIndex + i][xIndex + j] != 0)
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	bool Game::CheckIsPieceCanMove(Direction direction)
	{
		int xIndex = (currentFigurePosition.x - BOARD_POSITION_X) / BLOCK_SIZE + (int)direction;
		int yIndex = (currentFigurePosition.y - BOARD_POSITION_Y) / BLOCK_SIZE;

		for (int i{}; i < Figures[static_cast<int>(currentFigure)][rotation].size(); i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < Figures[static_cast<int>(currentFigure)][rotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(currentFigure)][rotation][i][j] != 0)
				{
					if (board[yIndex + i][xIndex + j] != 0)
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	PieceRotation Game::CheckIsPieceCanRotate()
	{
 		int xIndex{ (currentFigurePosition.x - BOARD_POSITION_X) / BLOCK_SIZE };
		int yIndex{ (currentFigurePosition.y - BOARD_POSITION_Y) / BLOCK_SIZE };
		int nextRotation{ CalculateNextRotation() };

		PieceRotation pieceRotaion{};

 		int leftShift
		{ 
			xIndex 
			+ 
			(int)Figures[static_cast<int>(currentFigure)][nextRotation][0].size() + 1
			- 
			(int)(BOARD_WIDTH / BLOCK_SIZE) 
		};

		if (leftShift < 0)
		{
			leftShift = 0;
		}

		xIndex -= leftShift;

		int rightShift{ 0 - xIndex + 1 };

		if (rightShift < 0)
		{
			rightShift = 0;
		}

		xIndex += rightShift;

		for (int i{}; i < Figures[static_cast<int>(currentFigure)][nextRotation].size(); i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < Figures[static_cast<int>(currentFigure)][nextRotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(currentFigure)][nextRotation][i][j] != 0)
				{
					if (board[yIndex + i][xIndex + j] != 0)
					{
						pieceRotaion.pieceCanRotate = false;
						return pieceRotaion;
					}
				}
			}
		}

		pieceRotaion.pieceShift -= leftShift;
		pieceRotaion.pieceShift += rightShift;

		pieceRotaion.nextRotation = nextRotation;
		pieceRotaion.pieceCanRotate = true;

		return pieceRotaion;
	}

	int Game::CalculateNextRotation()
	{
		if (rotation < PIECE_ROTATIONS - 1)
		{
			return rotation + 1;
		}
		else
		{
			return 0;
		}
	}

	void Game::SaveCurrentPiece()
	{
		int xIndex = (currentFigurePosition.x - BOARD_POSITION_X) / BLOCK_SIZE;
		int yIndex = (currentFigurePosition.y - BOARD_POSITION_Y) / BLOCK_SIZE;

		for (int i{}; i < Figures[static_cast<int>(currentFigure)][rotation].size(); i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < Figures[static_cast<int>(currentFigure)][rotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(currentFigure)][rotation][i][j] != 0)
				{
					board[yIndex + i][xIndex + j] = 2;
				}
			}
		}
	}

	void Game::DeleteLines()
	{
		bool isFullLine{ true };

		for (size_t i{ 1 }; i < board.capacity() - 1; i++)
		{
			for (size_t j{ 1 }; j < board[i].capacity() - 1; j++)
			{
				if (board[i][j] == 0)
				{
					isFullLine = false;
				}
			}

			if (isFullLine)
			{
				DropUpperBlocks(i);
				AddScore();
			}

			isFullLine = true;
		}
	}

	void Game::DropUpperBlocks(int yIndex)
	{
		for (int i{ yIndex }; i > 1; i--)
		{
			for (size_t j{ 1 }; j < board[i].capacity() - 1; j++)
			{
				board[i][j] = board[i - 1][j];
			}
		}

		for (size_t j{ 1 }; j < board[0].capacity() - 1; j++)
		{
			board[0][j] = 0;
		}
	}

	void Game::AddFrame()
	{
		if (currentFrame == FPS)
		{
			currentFrame = 0;
		}
		else
		{
			currentFrame++;
		}
	}

	void Game::CheckIsGameOver()
	{
		int xIndex = (currentFigurePosition.x - BOARD_POSITION_X) / BLOCK_SIZE;
		int yIndex = (currentFigurePosition.y - BOARD_POSITION_Y) / BLOCK_SIZE;

		for (int i{}; i < Figures[static_cast<int>(currentFigure)][rotation].size(); i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < Figures[static_cast<int>(currentFigure)][rotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(currentFigure)][rotation][i][j] != 0)
				{
					if (board[yIndex + i][xIndex + j] != 0)
					{
						gameState = GameState::GameOver;
					}
				}
			}
		}
	}

	void Game::DrawScene()
	{
		SDL_RenderCopy(renderer, infoBlockTexture, NULL, &INFO_BLOCK_RECT);

		int nextPieceWidth = (int)Figures[static_cast<int>(nextFigure)][nextRotation][0].size();
		int nextPiecePositionX{ 
			INFO_BLOCK_POSITION_X 
			+ 
			(INFO_BLOCK_WIDTH - BLOCK_SIZE * nextPieceWidth) / 2 };

		DrawFigure(nextFigure, nextRotation, nextPiecePositionX, NEXT_PIECE_POSITION_Y);

		int tempScore{ score };

		for (int i{ NUMBER_OF_SCORE_DIGITS - 1 }; i >= 0; i--)
		{
			CreateMessage
			(
				Font::Scene, 
				std::to_string(tempScore % 10).c_str(), 
				MAIN_FONT_COLOR,
				SCORE_MESSAGE_RECTANGLES[i]
			);

			tempScore /= 10;
		}
	}

	void Game::CreateMessage(
		Font fontKind,
		const char* text, 
		SDL_Color color, 
		SDL_Rect messageRectangle)
	{
		TTF_Font* font = GetFont(fontKind);

		SDL_Surface* surface{
			TTF_RenderText_Solid(font, text, color) 
		};

		if (surface == NULL)
		{
			throw SurfaceNullReference();
		}

		SDL_Texture* message{
			SDL_CreateTextureFromSurface(renderer, surface)
		};

		if (message == NULL)
		{
			throw MessageNullReference();
		}

		SDL_RenderCopy(renderer, message, NULL, &messageRectangle);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(message);
	}

	TTF_Font* Game::GetFont(Font font)
	{
		switch (font)
		{
		case Font::GameOver:
			return gameOverFont;

		case Font::Scene:
			return sceneFont;

		default: 
			return sceneFont;
		}
	}

	void Game::PrintGameOver()
	{
		SetColor(Color::transparentBlack);
		SDL_RenderFillRect(renderer, &BACKGROUND_RECTANGLE);

		CreateMessage(Font::GameOver, "GAME OVER!", MAIN_FONT_COLOR, GAME_OVER_MESSAGE_RECTANGLE);
		CreateMessage(Font::Scene, "Press Enter to start again", MAIN_FONT_COLOR, START_AGAIN_MESSAGE_RECTANGLE);
	}

	void Game::AddScore()
	{
		if (score + SCORE_ADDITION <= SCORE_MAX_VALUE)
		{
			score += SCORE_ADDITION;
		}
	}

	void Game::PrintPauseGame()
	{
		SetColor(Color::transparentBlack);
		SDL_RenderFillRect(renderer, &BACKGROUND_RECTANGLE);

		CreateMessage(Font::GameOver, "GAME PAUSED", MAIN_FONT_COLOR, GAME_OVER_MESSAGE_RECTANGLE);
		CreateMessage(Font::Scene, "Press Enter or Escape to resume game", MAIN_FONT_COLOR, START_AGAIN_MESSAGE_RECTANGLE);
	}

	SDL_Texture* Game::LoadTexture(const char* textureFilePath)
	{
		SDL_Surface* surface
		{
			IMG_Load(textureFilePath)
		};

		if (surface == NULL)
		{
			throw SurfaceNullReference();
		}

		SDL_Texture* texture
		{ 
			SDL_CreateTextureFromSurface(renderer, surface) 
		};

		if (texture == NULL)
		{
			throw TextureNullReference();
		}

		SDL_FreeSurface(surface);

		return texture;
	}
}
