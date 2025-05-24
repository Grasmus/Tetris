#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"
#include "DatabaseManager.h"
#include "TextInput.h"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace GameNamespace
{
	const char* SETTINGS_FILE_NAME{ "settings.txt" };
	const char* RESOLUTION_SETTING_NAME{ "Screen resolution" };
	const char* DEFAULT_DATABASE_PATH{ "tetris_users.db" };

	static std::vector<ScreenResolution> SCREEN_RESOLUTIONS
	{
		ScreenResolution{ "640x480", 640, 480 },
		ScreenResolution{ "800x600", 800, 600 },
		ScreenResolution{ "1366x768", 1366, 768 },
		ScreenResolution{ "1600x900", 1600, 900 },
		ScreenResolution{ "1920x1080", 1920, 1080 },
		ScreenResolution{ "3840x2160", 3840, 2160 }
	};

	static std::map<SettingsType, std::vector<SettingValue*>> settingValues{};

	Game* Game::game;

	Game::Game(): 
		settings(new Settings(SETTINGS_FILE_NAME)), 
		resourceHandler(new ResourceHandler()),
		dbManager(new DatabaseManager(DEFAULT_DATABASE_PATH))
	{
		CreateLayout();

		if (SDL_Init(SDL_INIT_EVERYTHING))
		{
			throw SDLInitException();
		}

		window = SDL_CreateWindow(
			GAME_WINDOW_NAME,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			layout->GetWindowWidth(),
			layout->GetWindowHeight(),
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

		resourceHandler->LoadFonts(renderer, layout->GetWindowWidth());
		resourceHandler->LoadTextures(renderer);

		if (SDL_SetTextureAlphaMod(resourceHandler->GetTexture(Texture::Board), 100))
		{
			throw SetTextureAlphaModException();
		}

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		srand(time(NULL));		

		UpdateTopScores();

		CreateUI();
	}

	Game::~Game()
	{
		resourceHandler.reset();

		TTF_Quit();

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

		case GameState::Menu:
			HandleGameEvent(event, UIDestination::Menu, GameState::Inactive);
			break;

		case GameState::Settings:
			HandleGameEvent(event, UIDestination::Settings, GameState::Menu);
			break;

		case GameState::Leaderboard:
			HandleGameEvent(event, UIDestination::Leaderboard, GameState::Menu);
			break;

		case GameState::EnterUsername:
			HandleGameEvent(event, UIDestination::EnterUsername, GameState::Menu);
			break;

		default:
			break;
		}
	}

	void Game::Render()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, resourceHandler->GetTexture(Texture::Background), NULL, NULL);

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

		case GameState::Menu:
			RenderElements(UIDestination::Menu);
			break;

		case GameState::Settings:
			RenderElements(UIDestination::Settings);
			break;

		case GameState::Leaderboard:
			RenderElements(UIDestination::Leaderboard);
			break;

		case GameState::EnterUsername:
			RenderElements(UIDestination::EnterUsername);
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

	bool Game::IsRunning() const
	{
		return gameState != GameState::Inactive;
	}

	void Game::DrawBlock(SDL_Point position, Color color)
	{
		SDL_Rect rect
		{
			position.x,
			position.y,
			layout->GetBlockSize(),
			layout->GetBlockSize()
		};

		SetColor(color);
		SDL_RenderFillRect(renderer, &rect);
	}

	void Game::DrawBlock(SDL_Point position, SDL_Texture* texture)
	{
		SDL_Rect rect
		{
			position.x,
			position.y,
			layout->GetBlockSize(),
			layout->GetBlockSize()
		};

		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}

	void Game::SetColor(Color color)
	{
		switch (color)
		{
		case Color::Red:
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			break;

		case Color::Green:
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			break;

		case Color::Black:
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			break;

		case Color::Blue:
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			break;

		case Color::White:
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			break;

		case Color::TransparentBlack:
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
			break;

		default:
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
	}

	int Game::GetFrameDelay() const
	{
		return FRAME_DELAY;
	}

	void Game::ResetFigurePosition()
	{
		currentFigurePosition = SDL_Point
		{
			layout->GetBoardRect().x + layout->GetPieceInitialShiftX(),
			layout->GetBoardRect().y
		};
	}

	void Game::DrawFigure()
	{
		int primalPosition{ currentFigurePosition.x };
		SDL_Point piecePosition{ currentFigurePosition };

		for (size_t i{}; i < Figures[static_cast<int>(currentFigure)][rotation].size(); i++)
		{
			for (size_t j{}; j < Figures[static_cast<int>(currentFigure)][rotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(currentFigure)][rotation][i][j] == 0)
				{
					piecePosition.x += layout->GetBlockSize();
				}
				else
				{
					DrawBlock(piecePosition, resourceHandler->GetTexture(Texture::Block));
					piecePosition.x += layout->GetBlockSize();
				}
			}

			piecePosition.y += layout->GetBlockSize();
			piecePosition.x = primalPosition;
		}
	}

	void Game::DrawFigure(FigureKind figure, size_t rotation, int x, int y)
	{
		int primalXPosition{ x };
		SDL_Point piecePosition{ x, y };

		for (size_t i{}; i < Figures[static_cast<int>(figure)][rotation].size(); i++)
		{
			for (size_t j{}; j < Figures[static_cast<int>(figure)][rotation][i].size(); j++)
			{
				if (Figures[static_cast<int>(figure)][rotation][i][j] == 0)
				{
					piecePosition.x += layout->GetBlockSize();
				}
				else
				{
					DrawBlock(piecePosition, resourceHandler->GetTexture(Texture::Block));
					piecePosition.x += layout->GetBlockSize();
				}
			}

			piecePosition.y += layout->GetBlockSize();
			piecePosition.x = primalXPosition;
		}
	}

	void Game::DrawBoard()
	{
		SDL_Rect drawBoardRect{ layout->GetDrawBoardRect() };

		SDL_RenderCopy(renderer, resourceHandler->GetTexture(Texture::Board), NULL, &drawBoardRect);
		int primalXPosition{ layout->GetBoardRect().x };
		SDL_Point blockPosition{ layout->GetBoardRect().x, layout->GetBoardRect().y };

		for (int i{}; i < BOARD_HEIGHT_IN_BLOCKS; i++)
		{
			for (int j{}; j < BOARD_WIDTH_IN_BLOCKS; j++)
			{
				switch (board[i][j])
				{
				case 3:
					break;

				case 2:
					DrawBlock(blockPosition, resourceHandler->GetTexture(Texture::Block));
					break;

				default:
					break;
				}

				blockPosition.x += layout->GetBlockSize();
			}

			blockPosition.x = primalXPosition;
			blockPosition.y += layout->GetBlockSize();
		}
	}

	void Game::HandleGameEvent(SDL_Event event, UIDestination destination, GameState exitState)
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

				HandleMouseLeftClick(mouseCoordinateX, mouseCoordinateY, destination);

				break;

			default:
				break;
			}

			break;
		case SDL_TEXTINPUT:

			HandleTextInput(event.text.text, destination);

			break;

		case SDL_KEYDOWN:

			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:

				gameState = exitState;

				break;

			default:
				break;
			}

			HandleKeyDown(event.key.keysym.sym, destination);

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
				gameState = GameState::Menu;
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
				StartGame();
				break;

			case SDLK_ESCAPE:
				gameState = GameState::Menu;
				isScoreSaved = false;
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
				currentFigurePosition.x -= layout->GetBlockSize();
			}
			break;

		case PieceMovement::Right:

			if (CheckIsPieceCanMove(Direction::Right))
			{
				currentFigurePosition.x += layout->GetBlockSize();
			}
			break;

		case PieceMovement::Rotation:

			pieceRotation = CheckIsPieceCanRotate();

			if (pieceRotation.pieceCanRotate)
			{
				currentFigurePosition.x += pieceRotation.pieceShift * layout->GetBlockSize();
				rotation = pieceRotation.nextRotation;
			}
			break;

		case PieceMovement::SpeedUp:

			if (CheckIsPieceCanMove())
			{
				currentFigurePosition.y += layout->GetBlockSize();
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
					currentFigurePosition.y += layout->GetBlockSize();
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

	void Game::EnterUsername()
	{
		game->gameState = GameState::EnterUsername;
	}

	void Game::StartGame()
	{
		if (game->username.empty()) 
		{
			return; 
		}

		game->gameStartTime = game->GetCurrentTimeString();
		game->score = 0;
		game->isScoreSaved = false;

		game->board = game->InitializeBoard();
		game->currentFigure = (FigureKind)(rand() % PIECE_KINDS);
		game->nextFigure = (FigureKind)(rand() % PIECE_KINDS);
		game->rotation = rand() % PIECE_ROTATIONS;
		game->nextRotation = rand() % PIECE_ROTATIONS;

		game->CreateLayout();

		game->gameState = GameState::Running;
	}

	void Game::EnterSettings()
	{
		game->gameState = GameState::Settings;
	}

	void Game::EnterLeaderboard()
	{
		game->gameState = GameState::Leaderboard;
	}

	void Game::EnterMenu()
	{
		game->gameState = GameState::Menu;
	}

	void Game::SaveSettings()
	{
		for (auto& uiElement : game->UIElements[UIDestination::Settings])
		{
			SettingsElement* settingsElement{ dynamic_cast<SettingsElement*>(uiElement.get()) };

			if (settingsElement != nullptr)
			{
				SettingsType settingType{ settingsElement->GetSettingType() };
				int currentValueIndex{ settingsElement->GetValueIndex() };

				game->settings->SetSetting(
					settingType, 
					settingValues[settingType][currentValueIndex]->GetLable().c_str()
				);
			}
		}

		game->settings->Save();
		game->CreateLayout();
		game->ChangeWindowSize();
		game->CreateUI();
		game->gameState = GameState::Menu;
	}

	Game* Game::Init()
	{
		settingValues[SettingsType::ScreenResolution] = std::vector<SettingValue*>();

		for (auto& screenResolution : SCREEN_RESOLUTIONS)
		{
			settingValues[SettingsType::ScreenResolution].push_back(&screenResolution);
		}

		if (!game)
		{
			game = new Game();

			return game;
		}

		throw GameAlreadyCreatedException();
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
			
			ResetFigurePosition();
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
		int xIndex = (currentFigurePosition.x - layout->GetBoardRect().x) / layout->GetBlockSize();
		int yIndex = (currentFigurePosition.y - layout->GetBoardRect().y) / layout->GetBlockSize() + 1;

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
		int xIndex = (currentFigurePosition.x - layout->GetBoardRect().x) / layout->GetBlockSize() + (int)direction;
		int yIndex = (currentFigurePosition.y - layout->GetBoardRect().y) / layout->GetBlockSize();

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
 		int xIndex{ (currentFigurePosition.x - layout->GetBoardRect().x) / layout->GetBlockSize() };
		int yIndex{ (currentFigurePosition.y - layout->GetBoardRect().y) / layout->GetBlockSize() };
		int nextRotation{ CalculateNextRotation() };

		PieceRotation pieceRotaion{};

 		int leftShift
		{ 
			xIndex 
			+ 
			(int)Figures[static_cast<int>(currentFigure)][nextRotation][0].size() + 1
			- 
			(int)(layout->GetBoardRect().w / layout->GetBlockSize())
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

	int Game::CalculateNextRotation() const
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
		int xIndex = (currentFigurePosition.x - layout->GetBoardRect().x) / layout->GetBlockSize();
		int yIndex = (currentFigurePosition.y - layout->GetBoardRect().y) / layout->GetBlockSize();

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
		int xIndex = (currentFigurePosition.x - layout->GetBoardRect().x) / layout->GetBlockSize();
		int yIndex = (currentFigurePosition.y - layout->GetBoardRect().y) / layout->GetBlockSize();

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

						if (!isScoreSaved && dbManager && !username.empty() && !gameStartTime.empty()) 
						{
							dbManager->addUser(username, gameStartTime, score);
							isScoreSaved = true;
							UpdateTopScores();
						}
					}
				}
			}
		}
	}

	void Game::DrawScene()
	{
		SDL_Rect infoBlockRect{ layout->GetInfoBlockRect() };

		SDL_RenderCopy(renderer, resourceHandler->GetTexture(Texture::InfoBlock), NULL, &infoBlockRect);

		int nextPieceWidth = (int)Figures[static_cast<int>(nextFigure)][nextRotation][0].size();
		int nextPiecePositionX{ 
			layout->GetInfoBlockRect().x
			+ 
			(layout->GetInfoBlockRect().w - layout->GetBlockSize() * nextPieceWidth) / 2 };

		DrawFigure(nextFigure, nextRotation, nextPiecePositionX, layout->GetNextPiecePositionY());

		std::string scoreStr{ std::to_string(score) };

		scoreStr.insert(0, NUMBER_OF_SCORE_DIGITS - scoreStr.length(), '0');

		const char* scoreCStr{ scoreStr.c_str() };

		SDL_Rect scoreDimensions
		{
			CalcTextDimensions(Font::Score, scoreCStr)
		};

		SDL_Point scorePosition
		{
			layout->GetInfoBlockRect().x + (layout->GetInfoBlockRect().w - scoreDimensions.w) / 2,
			layout->GetInfoBlockRect().y + layout->GetScoreYOffset()
		};

		CreateMessage(
			Font::Score,
			scoreCStr,
			MAIN_FONT_COLOR,
			scoreDimensions.w,
			scoreDimensions.h,
			scorePosition
		);
	}

	void Game::CreateMessage(
		Font fontType,
		const char* text, 
		Color color, 
		int width, 
		int height, 
		SDL_Point position)
	{
		TTF_Font* font = resourceHandler->GetFont(fontType);

		SDL_Surface* surface
		{
			TTF_RenderText_Solid(font, text, GetColor(color)) 
		};

		if (surface == NULL)
		{
			throw SurfaceNullReference();
		}

		SDL_Texture* message
		{
			SDL_CreateTextureFromSurface(renderer, surface)
		};

		if (message == NULL)
		{
			throw MessageNullReference();
		}

		SDL_Rect messageRect
		{
			position.x,
			position.y,
			width,
			height
		};

		SDL_RenderCopy(renderer, message, NULL, &messageRect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(message);
	}

	void Game::PrintGameOver()
	{
		SDL_Rect backgroundRect{ layout->GetBackgroundRect() };

		SetColor(Color::TransparentBlack);
		SDL_RenderFillRect(renderer, &backgroundRect);

		SDL_Rect gameOverDimensions
		{
			CalcTextDimensions(Font::Main, GAME_OVER_MESSAGE)
		};

		SDL_Point gameOverMessagePosition
		{
			(layout->GetWindowWidth() - gameOverDimensions.w) / 2,
			(layout->GetWindowHeight() - gameOverDimensions.h) / 2 - layout->GetBlockSize() / 2
		};

		SDL_Rect startAgainDimensions
		{
			CalcTextDimensions(Font::Scene, START_AGAIN_MESSAGE)
		};

		SDL_Point startAgainMessagePosition
		{
			(layout->GetWindowWidth() - startAgainDimensions.w) / 2,
			(layout->GetWindowHeight() - startAgainDimensions.h) / 2
			+ 
			(gameOverDimensions.h + layout->GetBlockSize()) / 2
		};

		CreateMessage(
			Font::Main, 
			GAME_OVER_MESSAGE, 
			MAIN_FONT_COLOR, 
			gameOverDimensions.w,
			gameOverDimensions.h,
			gameOverMessagePosition
		);

		CreateMessage(
			Font::Scene, 
			START_AGAIN_MESSAGE, 
			MAIN_FONT_COLOR,
			startAgainDimensions.w,
			startAgainDimensions.h,
			startAgainMessagePosition
		);
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
		SDL_Rect backgroundRect{ layout->GetBackgroundRect() };

		SetColor(Color::TransparentBlack);
		SDL_RenderFillRect(renderer, &backgroundRect);

		SDL_Rect gamePausedDimensions
		{
			CalcTextDimensions(Font::Main, GAME_PUSED_HEADER)
		};

		SDL_Point gamePauseHeaderPosition
		{
			(layout->GetWindowWidth() - gamePausedDimensions.w) / 2,
			(layout->GetWindowHeight() - gamePausedDimensions.h) / 2 - layout->GetBlockSize() / 2
		};

		SDL_Rect gamePausedMessageDimensions
		{
			CalcTextDimensions(Font::Scene, GAME_PUSED_MESSAGE)
		};

		SDL_Point gamePauseMessagePosition
		{
			(layout->GetWindowWidth() - gamePausedMessageDimensions.w) / 2,
			(layout->GetWindowHeight() - gamePausedMessageDimensions.h) / 2
			+ 
			(gamePausedDimensions.h + layout->GetBlockSize()) / 2
		};

		CreateMessage(
			Font::Main, 
			GAME_PUSED_HEADER, 
			MAIN_FONT_COLOR, 
			gamePausedDimensions.w,
			gamePausedDimensions.h,
			gamePauseHeaderPosition
		);

		CreateMessage(
			Font::Scene, 
			GAME_PUSED_MESSAGE,
			MAIN_FONT_COLOR, 
			gamePausedMessageDimensions.w,
			gamePausedMessageDimensions.h,
			gamePauseMessagePosition
		);
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

	void Game::CreateLayout()
	{
		std::string screenResolution{ settings->GetSetting(SettingsType::ScreenResolution) };

		int windowWidth{ DEFAULT_WINDOW_WIDTH }, windowHeight{ DEFAULT_WINDOW_HEIGHT };

		for (int i{}; i < settingValues[SettingsType::ScreenResolution].size(); i++)
		{
			if (settingValues[SettingsType::ScreenResolution][i]->GetLable().compare(screenResolution) == 0)
			{
				ScreenResolution* screenResolution = 
					static_cast<ScreenResolution*>(settingValues[SettingsType::ScreenResolution][i]);

				windowWidth = screenResolution->GetWidth();
				windowHeight = screenResolution->GetHeight();
			}
		}

		layout = std::make_unique<Layout>(windowWidth, windowHeight);

		ResetFigurePosition();
	}

	unsigned Game::CalcRelativeWidth(unsigned width)
	{
		return width * layout->GetWindowWidth() / DEFAULT_WINDOW_WIDTH;
	}

	unsigned Game::CalcRelativeHeight(unsigned height)
	{
		return height * layout->GetWindowHeight() / DEFAULT_WINDOW_HEIGHT;
	}

	SDL_Color Game::GetColor(Color color)
	{
		switch (color)
		{
		case Color::Red:
			return SDL_Color{ 255, 0, 0, 255 };

		case Color::Green:
			return SDL_Color{ 0, 255, 0, 255 };

		case Color::Black:
			return SDL_Color{ 0, 0, 0, 255 };

		case Color::Blue:
			return SDL_Color{ 0, 0, 255, 255 };

		case Color::White:
			return SDL_Color{ 255, 255, 255, 255 };

		case Color::TransparentBlack:
			return SDL_Color{ 0, 0, 0, 150 };

		default:
			return SDL_Color{ 255, 255, 255, 255 };
		}
	}

	SDL_Rect Game::CalcTextDimensions(Font fontType, const char* text)
	{
		int textWidth{}, textHeight{};

		if (TTF_SizeText(resourceHandler->GetFont(fontType), text, &textWidth, &textHeight))
		{
			throw TTFSizeTextException();
		}

		return { 0, 0, textWidth, textHeight };
	}

	void Game::CreateUI()
	{
		UIElements.clear();

		SDL_Rect titleRect
		{
			CalcTextDimensions(Font::Title, TITLE_TEXT)
		};

		titleRect.x = (layout->GetWindowWidth() - titleRect.w) / 2;
		titleRect.y = layout->GetWindowHeight() / 3 - titleRect.h / 2;

		AddTextView(
			titleRect,
			TITLE_TEXT,
			Font::Title,
			Color::Red,
			UIDestination::Menu
		);

		SDL_Rect playButtonRect
		{
			CalcTextDimensions(Font::Button, PLAY_BUTTON_TEXT)
		};

		playButtonRect.x = (layout->GetWindowWidth() - playButtonRect.w) / 2;
		playButtonRect.y = (layout->GetWindowHeight() - playButtonRect.h) / 2 + playButtonRect.h / 2;

		AddButton(
			playButtonRect,
			Font::Button,
			MAIN_FONT_COLOR,
			PLAY_BUTTON_TEXT,
			EnterUsername,
			UIDestination::Menu
		);

		SDL_Rect textInputRect
		{
			0, 0,
			TEXT_INPUT_RECT.w,
			TEXT_INPUT_RECT.h
		};

		textInputRect.x = (layout->GetWindowWidth() - textInputRect.w) / 2;
		textInputRect.y = 
			(layout->GetWindowHeight() - textInputRect.h) / 2 - textInputRect.h;

		AddTextInput(
			textInputRect,
			Font::TextInput,
			Color::White,
			Color::Red,
			USERNAME_MAX_LENGTH,
			UpdateUsernameFromTextInput,
			UIDestination::EnterUsername
		);

		SDL_Rect startGameButtonRect
		{
			CalcTextDimensions(Font::Button, START_GAME_BUTTON_TEXT)
		};

		startGameButtonRect.x = (layout->GetWindowWidth() - startGameButtonRect.w) / 2;
		startGameButtonRect.y = 
			(layout->GetWindowHeight() - startGameButtonRect.h) / 2 + startGameButtonRect.h;

		AddButton(
			startGameButtonRect,
			Font::Button,
			MAIN_FONT_COLOR,
			START_GAME_BUTTON_TEXT,
			StartGame,
			UIDestination::EnterUsername
		);

		SDL_Rect settingsButtonRect
		{
			CalcTextDimensions(Font::Button, SETTINGS_BUTTON_TEXT)
		};

		settingsButtonRect.x = (layout->GetWindowWidth() - settingsButtonRect.w) / 2;
		settingsButtonRect.y = playButtonRect.y + playButtonRect.h;

		AddButton(
			settingsButtonRect,
			Font::Button,
			MAIN_FONT_COLOR,
			SETTINGS_BUTTON_TEXT,
			EnterSettings,
			UIDestination::Menu
		);

		SDL_Rect leaderboardButtonRect
		{
			CalcTextDimensions(Font::Button, LEADERBOARD_BUTTON_TEXT)
		};

		leaderboardButtonRect.x = (layout->GetWindowWidth() - leaderboardButtonRect.w) / 2;
		leaderboardButtonRect.y = settingsButtonRect.y + settingsButtonRect.h;

		AddButton(
			leaderboardButtonRect,
			Font::Button,
			MAIN_FONT_COLOR,
			LEADERBOARD_BUTTON_TEXT,
			EnterLeaderboard,
			UIDestination::Menu
		);

		std::vector<std::string> resolutionLables{};

		for (int i{}; i < settingValues[SettingsType::ScreenResolution].size(); i++)
		{
			ScreenResolution* screenResolution =
				static_cast<ScreenResolution*>(settingValues[SettingsType::ScreenResolution][i]);

			resolutionLables.push_back(screenResolution->GetLable());
		}

		SDL_Point resolutionSettingPosition
		{
			RESOLUTION_SETTING_POSITION_SCALERS.x * layout->GetBlockSize(),
			RESOLUTION_SETTING_POSITION_SCALERS.y * layout->GetBlockSize()
		};

		int currentResolutionIndex{};

		for (; currentResolutionIndex < settingValues[SettingsType::ScreenResolution].size(); currentResolutionIndex++)
		{
			ScreenResolution* screenResolution =
				static_cast<ScreenResolution*>(settingValues[SettingsType::ScreenResolution][currentResolutionIndex]);

			if (screenResolution->GetLable()
				.compare(settings->GetSetting(SettingsType::ScreenResolution)) == 0)
			{
				break;
			}
		}

		AddSettingsElement(
			resolutionSettingPosition,
			layout->GetWindowWidth() - RESOLUTION_SETTING_POSITION_SCALERS.x * 2 * layout->GetBlockSize(),
			SettingsType::ScreenResolution,
			RESOLUTION_SETTING_NAME,
			resolutionLables,
			currentResolutionIndex,
			Font::Button,
			Color::Red
		);

		SDL_Rect saveSettingsButtonRect
		{
			CalcTextDimensions(Font::Button, SETTINGS_SAVE_BUTTON_TEXT)
		};

		saveSettingsButtonRect.x = (layout->GetWindowWidth() - playButtonRect.w) / 2;
		saveSettingsButtonRect.y = 
			layout->GetWindowHeight() - SAVE_SETTINGS_BUTTON_Y_OFFSET_SCALER * layout->GetBlockSize();

		AddButton(
			saveSettingsButtonRect,
			Font::Button,
			MAIN_FONT_COLOR,
			SETTINGS_SAVE_BUTTON_TEXT,
			SaveSettings,
			UIDestination::Settings
		);

		SDL_Rect leaderboardRect
		{
			(layout->GetWindowWidth() - LEADERBOARD_WIDTH_IN_BLOCKS * layout->GetBlockSize()) / 2,
			LEADERBOARD_Y_POSITION_IN_BLOCKS * layout->GetBlockSize(),
			LEADERBOARD_WIDTH_IN_BLOCKS* layout->GetBlockSize(),
			0
		};

		AddLeaderboard(
			leaderboardRect,
			LEADERBOARD_PADDING,
			topScores,
			Font::Scene,
			Color::White,
			Color::TransparentBlack
		);

		SDL_Rect leaderboardBackButton
		{
			CalcTextDimensions(Font::Button, LEADERBOARD_BACK_BUTTON_TEXT)
		};

		leaderboardBackButton.x = (layout->GetWindowWidth() - leaderboardBackButton.w) / 2;
		leaderboardBackButton.y = 
			leaderboardRect.y + UIElements[UIDestination::Leaderboard].back()->GetRect().h + layout->GetBlockSize();

		AddButton(
			leaderboardBackButton,
			Font::Button,
			Color::Red,
			LEADERBOARD_BACK_BUTTON_TEXT,
			EnterMenu,
			UIDestination::Leaderboard
		);
	}

	void Game::ChangeWindowSize()
	{
		SDL_SetWindowSize(window, layout->GetWindowWidth(), layout->GetWindowHeight());

		resourceHandler->LoadFonts(renderer, layout->GetWindowWidth());
	}

	void Game::AddButton(
		SDL_Rect buttonRect,
		Font font,
		Color color,
		const char* text,
		void (*function)(),
		UIDestination destination)
	{
		if (UIElements.find(destination) == UIElements.end())
		{
			UIElements[destination] = std::vector<std::unique_ptr<UIElement>>();
		}

		UIElements[destination].push_back(
			std::make_unique<Button>(
				buttonRect,
				renderer,
				text,
				resourceHandler->GetFont(font),
				GetColor(color),
				function
			)
		);
	}

	void Game::AddTextInput(
		SDL_Rect textInputRect,
		Font font,
		Color textColor,
		Color caretteColor,
		int textMaxLenght,
		void (*onTextInput)(std::string&),
		UIDestination destination)
	{
		if (UIElements.find(destination) == UIElements.end())
		{
			UIElements[destination] = std::vector<std::unique_ptr<UIElement>>();
		}

		SDL_Rect textInputRelativeRect
		{
			textInputRect.x,
			textInputRect.y,
			CalcRelativeWidth(textInputRect.w),
			CalcRelativeHeight(textInputRect.h)
		};

		UIElements[destination].push_back(
			std::make_unique<TextInput>(
				textInputRelativeRect,
				resourceHandler->GetTexture(Texture::TextInput),
				resourceHandler->GetFont(font),
				GetColor(textColor),
				GetColor(caretteColor),
				textMaxLenght,
				onTextInput
			)
		);
	}

	void Game::AddTextView(
		SDL_Rect textViewRect, 
		const char* text, 
		Font font, 
		Color textColor, 
		UIDestination destination)
	{
		if (UIElements.find(destination) == UIElements.end())
		{
			UIElements[destination] = std::vector<std::unique_ptr<UIElement>>();
		}

		UIElements[destination].push_back(
			std::make_unique<TextView>(
				textViewRect,
				resourceHandler->GetFont(font),
				renderer,
				GetColor(textColor),
				text
			)
		);
	}

	void Game::AddSettingsElement(
		SDL_Point position, 
		int width, 
		SettingsType settingType,
		const char* settingName, 
		std::vector<std::string> values, 
		int currentValueIndex, 
		Font font,
		Color textColor)
	{
		if (UIElements.find(UIDestination::Settings) == UIElements.end())
		{
			UIElements[UIDestination::Settings] = std::vector<std::unique_ptr<UIElement>>();
		}

		UIElements[UIDestination::Settings].push_back(
			std::make_unique<SettingsElement>(
				position,
				width,
				renderer,
				settingType,
				settingName,
				values,
				currentValueIndex,
				resourceHandler->GetFont(font),
				GetColor(textColor)
			)
		);
	}

	void Game::AddLeaderboard(
		SDL_Rect rect, 
		int padding, 
		std::vector<ScoreEntry> topScores, 
		Font font, 
		Color textColor, 
		Color backgroundColor)
	{
		if (UIElements.find(UIDestination::Leaderboard) == UIElements.end())
		{
			UIElements[UIDestination::Leaderboard] = std::vector<std::unique_ptr<UIElement>>();
		}

		UIElements[UIDestination::Leaderboard].push_back(
			std::make_unique<Leaderboard>(
				rect,
				renderer,
				padding,
				topScores,
				resourceHandler->GetFont(font),
				GetColor(textColor),
				GetColor(backgroundColor)
			)
		);
	}

	void Game::RenderElements(UIDestination destination)
	{
		for (int elementIndex{}; elementIndex < UIElements[destination].size(); elementIndex++)
		{
			UIElements[destination][elementIndex]->Render(renderer);
		}
	}

	void Game::UpdateTopScores()
	{
		if (dbManager) 
		{
			topScores = dbManager->getTopScores(DEFAULT_SCORE_LIMIT);

			for (auto& uiElement : UIElements[UIDestination::Leaderboard])
			{
				Leaderboard* leaderboard = dynamic_cast<Leaderboard*>(uiElement.get());

				if (leaderboard)
				{
					leaderboard->UpdateTopScores(topScores);
				}
			}
		}
	}

	void Game::HandleMouseLeftClick(int mouseCoordinateX, int mouseCoordinateY, UIDestination destination)
	{
		for (int elementIndex{}; elementIndex < UIElements[destination].size(); elementIndex++)
		{
			UIElements[destination][elementIndex]->HandleMouseLeftClick(
				{
					mouseCoordinateX,
					mouseCoordinateY
				}
			);
		}
	}

	void Game::HandleTextInput(const char* text, UIDestination destination)
	{
		for (int elementIndex{}; elementIndex < UIElements[destination].size(); elementIndex++)
		{
			UIElements[destination][elementIndex]->HandleTextInput(text);
		}
	}

	void Game::UpdateUsernameFromTextInput(std::string& text)
	{
		game->username = text;
	}

	void Game::HandleKeyDown(SDL_Keycode keyCode, UIDestination destination)
	{
		for (int elementIndex{}; elementIndex < UIElements[destination].size(); elementIndex++)
		{
			UIElements[destination][elementIndex]->HandleKeyDown(keyCode);
		}
	}

	std::string Game::GetCurrentTimeString() 
	{
		std::time_t now{ std::time(nullptr) };
		std::tm* tmPtr{ std::localtime(&now) };
		std::ostringstream oss{};

		oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");

		return oss.str();
	}
}
