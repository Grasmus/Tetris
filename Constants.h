#pragma once
#include <SDL_ttf.h>
#include <vector>

namespace GameNamespace
{
    unsigned short const
        BOARD_WIDTH{ 220 },
        BOARD_HEIGHT{ 400 },
        BLOCK_SIZE{ 20 },
        BOARD_POSITION_X{ 90 },
        BOARD_POSITION_Y{ 100 },
        TIME_DELAY{ 5 },
        WINDOW_HEIGHT{ 600 },
        WINDOW_WIDTH{ 500 },
        PIECE_KINDS{ 7 },
        PIECE_ROTATIONS{ 4 },
        PIECES_SIZE{ 5 },
        PIECE_INITIAL_SHIFT_X{ BLOCK_SIZE * 3 },
        PIECE_INITIAL_SHIFT_Y{ BLOCK_SIZE * 2 },
        FPS{ 60 },
        FRAME_DELAY{ 400 / FPS },
        NEXT_PIECE_POSITION_X{ 350 },
        NEXT_PIECE_POSITION_Y{ 250 },
        YOUR_SCORE_POSITION_X{ 330 },
        YOUR_SCORE_POSITION_Y{ 110 },
        SCORE_ADDITION{ 1 },
        NUMBER_OF_SCORE_DIGITS{ 5 },
        DIGITS_GAP{ 25 };

    const int SCORE_MAX_VALUE{ 99999 };

    int const mPieces[PIECE_KINDS][PIECE_ROTATIONS][PIECES_SIZE][PIECES_SIZE] =
    {
        // Square
          {
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 1, 2, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 1, 2, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 0, 0, 0}
            }
           },

        // I
          {
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 1, 1},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {1, 1, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}
            }
           }
          ,
        // L
          {
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
            }
           },
        // L mirrored
          {
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0}
            }
           },
        // N
          {
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0}
            },



           {
            {0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
            }
           },
        // N mirrored
          {
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 1, 2, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
            }
           },
        // T
          {
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}
            },
           {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
            }
           }
    };

    const char* const GAME_WINDOW_NAME = "Tetris";

    const char* const FONT_FILE_PATH = "./FFFFORWA.TTF";
    const int GAMEOVER_FONT_SIZE = 24;
    const int SCENE_FONT_SIZE = 24;
    const SDL_Color 
        MAIN_FONT_COLOR{ 255, 0, 0 },
        SECONDARY_FONT_COLOR{ 255, 255, 255 };

    const SDL_Rect GAME_OVER_MESSAGE_RECTANGLE
    {
        50, 200, 300, 100
    };

    const SDL_Rect START_AGAIN_MESSAGE_RECTANGLE
    {
        70, 300, 250, 35
    };

    const SDL_Rect NEXT_PIECE_MESSAGE_RECTANGLE
    {
        NEXT_PIECE_POSITION_X - BLOCK_SIZE,
        NEXT_PIECE_POSITION_Y - 2 * BLOCK_SIZE,
        150,
        35
    };

    const SDL_Rect YOUR_SCORE_MESSAGE_RECTANGLE
    {
        YOUR_SCORE_POSITION_X,
        YOUR_SCORE_POSITION_Y,
        150,
        35
    };

    const std::vector<SDL_Rect> SCORE_MESSAGE_RECTANGLES
    {
        {
            YOUR_SCORE_POSITION_X,
            YOUR_SCORE_POSITION_Y + 2 * BLOCK_SIZE,
            30,
            30
        },
        {
            YOUR_SCORE_POSITION_X + DIGITS_GAP,
            YOUR_SCORE_POSITION_Y + 2 * BLOCK_SIZE,
            30,
            30
        },
        {
            YOUR_SCORE_POSITION_X + 2 * DIGITS_GAP,
            YOUR_SCORE_POSITION_Y + 2 * BLOCK_SIZE,
            30,
            30
        },
        {
            YOUR_SCORE_POSITION_X + 3 * DIGITS_GAP,
            YOUR_SCORE_POSITION_Y + 2 * BLOCK_SIZE,
            30,
            30
        },
        {
            YOUR_SCORE_POSITION_X + 4 * DIGITS_GAP,
            YOUR_SCORE_POSITION_Y + 2 * BLOCK_SIZE,
            30,
            30
        }
    };

    enum class Color {
        red,
        green,
        blue,
        black,
        white
    };

    enum class Figure
    {
        Square = 0,
        I = 1,
        L = 2,
        L_mirrored = 3,
        N = 4,
        N_mirrored = 5,
        T = 6
    };

    enum class Font
    {
        GameOver,
        Scene
    };

    enum class Direction
    {
        Left = -1,
        Right = 1
    };

    const Color BACKGROUND_COLOR{ Color::black };
}
