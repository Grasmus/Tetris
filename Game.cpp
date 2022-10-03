#include "Game.h"
#include "GameExceptions.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string>

namespace GameNamespace
{
	Game::Game()
	{
		if (!SDL_Init(SDL_INIT_EVERYTHING))
		{
			window = SDL_CreateWindow(GAME_WINDOW_NAME, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

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
				SDL_Quit();
				exit(1);
			}

			gameOverFont = TTF_OpenFont(FONT_FILE_PATH, GAMEOVER_FONT_SIZE);
			sceneFont = TTF_OpenFont(FONT_FILE_PATH, SCENE_FONT_SIZE);

			if (gameOverFont == NULL || sceneFont == NULL)
			{
				throw FontNullReference();
			}

			srand(time(NULL));

			isRunning = true;
			board = InitializeBoard();
			currentFigure = (Figure)(rand() % PIECE_KINDS);
			nextFigure = (Figure)(rand() % PIECE_KINDS);
			rotation = rand() % PIECE_ROTATIONS;
			nextRotation = rand() % PIECE_ROTATIONS;
		}
		else
		{
			throw SDLInitException();
		}
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

		if (!isGameOver)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_a:
					leftMove = true;
					break;

				case SDLK_LEFT:
					leftMove = true;
					break;

				case SDLK_d:
					rightMove = true;
					break;

				case SDLK_RIGHT:
					rightMove = true;
					break;

				case SDLK_SPACE:
					rotationMove = true;
					break;

				case SDLK_s:
					speedUpMove = true;
					break;

				case SDLK_DOWN:
					speedUpMove = true;
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}
		}
		else
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_RETURN:
					startAgain = true;
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}
		}
	}

	void Game::Render()
	{
		SetColor(BACKGROUND_COLOR);
		SDL_RenderClear(renderer);
		DrawBoard();
		DrawFigure();
		DrawScene();

		if (isGameOver)
		{
			PrintGameOver();
		}

		SDL_RenderPresent(renderer);
	}

	void Game::Update()
	{
		if (!isGameOver)
		{
			if (leftMove && CheckIsPieceCanMove(Direction::Left))
			{
				currentFigurePosition.x -= BLOCK_SIZE;
				leftMove = false;
			}
			else if (rightMove && CheckIsPieceCanMove(Direction::Right))
			{
				currentFigurePosition.x += BLOCK_SIZE;
				rightMove = false;
			}
			else if (rotationMove && CheckIsPieceCanRotate())
			{
				rotation = CalculateNextRotation();
				rotationMove = false;
			}
			else if (CheckIsPieceCanMove())
			{
				if (currentFrame == FPS || speedUpMove)
				{
					currentFigurePosition.y += BLOCK_SIZE;
					speedUpMove = false;
				}
			}
			else
			{
				SaveCurrentPiece();

				currentFigure = nextFigure;
				nextFigure = (Figure)(rand() % PIECE_KINDS);
				rotation = nextRotation;
				nextRotation = rand() % PIECE_ROTATIONS;
				currentFigurePosition = {
					BOARD_POSITION_X + PIECE_INITIAL_SHIFT_X,
					BOARD_POSITION_Y - PIECE_INITIAL_SHIFT_Y
				};

				DeleteLines();
				CheckIsGameOver();
			}
		}
		else
		{
			if (startAgain)
			{
				board = InitializeBoard();
				currentFigure = (Figure)(rand() % PIECE_KINDS);
				nextFigure = (Figure)(rand() % PIECE_KINDS);
				rotation = rand() % PIECE_ROTATIONS;
				nextRotation = rand() % PIECE_ROTATIONS;
				currentFigurePosition = {
					BOARD_POSITION_X + PIECE_INITIAL_SHIFT_X,
					BOARD_POSITION_Y - PIECE_INITIAL_SHIFT_Y
				};

				isGameOver = false;
				startAgain = false;
				score = 0;
			}
		}
	}

	bool Game::Running()
	{
		return isRunning;
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
		SDL_RenderDrawRect(renderer, &rect);
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

		for (int i = 0; i < PIECES_SIZE; i++)
		{
			for (int j = 0; j < PIECES_SIZE; j++)
			{
				if (mPieces[static_cast<int>(currentFigure)][rotation][i][j] == 0)
				{
					piecePosition.x += BLOCK_SIZE;
				}
				else
				{
					DrawBlock(piecePosition, Color::green);
					piecePosition.x += BLOCK_SIZE;
				}
			}

			piecePosition.y += BLOCK_SIZE;
			piecePosition.x = primalPosition;
		}
	}

	void Game::DrawFigure(Figure figure, size_t rotation, int x, int y)
	{
		int primalPosition{ x };
		POINT piecePosition{ x, y };

		for (int i = 0; i < PIECES_SIZE; i++)
		{
			for (int j = 0; j < PIECES_SIZE; j++)
			{
				if (mPieces[static_cast<int>(figure)][rotation][i][j] == 0)
				{
					piecePosition.x += BLOCK_SIZE;
				}
				else
				{
					DrawBlock(piecePosition, Color::green);
					piecePosition.x += BLOCK_SIZE;
				}
			}

			piecePosition.y += BLOCK_SIZE;
			piecePosition.x = primalPosition;
		}
	}

	void Game::DrawBoard()
	{
		int initialX{ boardPosition.x };
		POINT block_position{ boardPosition };

		for (int i{}; i < BOARD_HEIGHT / BLOCK_SIZE; i++)
		{
			for (int j{}; j < BOARD_WIDTH / BLOCK_SIZE; j++)
			{
				switch (board[i][j])
				{
				case 3:
					DrawBlock(block_position, Color::white);
					break;

				case 2:
					DrawBlock(block_position, Color::green);
					break;

				default:
					break;
				}
				block_position.x += BLOCK_SIZE;
			}
			block_position.x = initialX;
			block_position.y += BLOCK_SIZE;
		}
	}

	std::vector<std::vector<int>> Game::InitializeBoard()
	{
		int boardHeightInBlocks = BOARD_HEIGHT / BLOCK_SIZE;
		int boardWidthinBlocks = BOARD_WIDTH / BLOCK_SIZE;
		std::vector<std::vector<int>> board(boardHeightInBlocks, std::vector<int>(boardWidthinBlocks));

		for (int i = 0; i < boardHeightInBlocks; i++)
		{
			for (int j = 0; j < boardWidthinBlocks; j++)
			{
				if (j == 0 || i == boardHeightInBlocks - 1 || j == boardWidthinBlocks - 1)
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

		for (int i{}; i < PIECES_SIZE; i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < PIECES_SIZE; j++)
			{
				if (mPieces[static_cast<int>(currentFigure)][rotation][i][j] != 0)
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

		for (int i{}; i < PIECES_SIZE; i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < PIECES_SIZE; j++)
			{
				if (mPieces[static_cast<int>(currentFigure)][rotation][i][j] != 0)
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

	bool Game::CheckIsPieceCanRotate()
	{
		int xIndex = (currentFigurePosition.x - BOARD_POSITION_X) / BLOCK_SIZE;
		int yIndex = (currentFigurePosition.y - BOARD_POSITION_Y) / BLOCK_SIZE;
		int nextRotation = CalculateNextRotation();

		for (int i{}; i < PIECES_SIZE; i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < PIECES_SIZE; j++)
			{
				if (mPieces[static_cast<int>(currentFigure)][nextRotation][i][j] != 0)
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

		for (int i{}; i < PIECES_SIZE; i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < PIECES_SIZE; j++)
			{
				if (mPieces[static_cast<int>(currentFigure)][rotation][i][j] != 0)
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

		for (int i{}; i < PIECES_SIZE; i++)
		{
			if (yIndex + i < 0)
			{
				continue;
			}

			for (int j{}; j < PIECES_SIZE; j++)
			{
				if (mPieces[static_cast<int>(currentFigure)][rotation][i][j] != 0)
				{
					if (board[yIndex + i][xIndex + j] != 0)
					{
						isGameOver = true;
					}
				}
			}
		}
	}

	void Game::DrawScene()
	{
		DrawFigure(nextFigure, nextRotation, NEXT_PIECE_POSITION_X, NEXT_PIECE_POSITION_Y);

		CreateMessage(Font::Scene, "Next piece:", MAIN_FONT_COLOR, NEXT_PIECE_MESSAGE_RECTANGLE);
		CreateMessage(Font::Scene, "Your score:", MAIN_FONT_COLOR, YOUR_SCORE_MESSAGE_RECTANGLE);

		int temp_score{ score };

		for (int i{ NUMBER_OF_SCORE_DIGITS - 1 }; i >= 0; i--)
		{
			CreateMessage(
				Font::Scene, 
				std::to_string(temp_score % 10).c_str(), 
				SECONDARY_FONT_COLOR, 
				SCORE_MESSAGE_RECTANGLES[i]);

			temp_score /= 10;
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
}
