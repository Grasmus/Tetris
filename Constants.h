#pragma once
#include <SDL_ttf.h>
#include <vector>

namespace GameNamespace
{
    typedef std::vector<std::vector<std::vector<std::vector<int>>>> FigureContainer;
    typedef std::vector<std::vector<int>> Figure;

    const int
        WINDOW_WIDTH{ 1920 },
        WINDOW_HEIGHT{ 1080 },

        BLOCK_SIZE{ WINDOW_WIDTH / 48 },

        BOARD_WIDTH_IN_BLOCKS{ 11 },
        BOARD_HEIGHT_IN_BLOCKS{ 23 },

        BOARD_WIDTH{ BOARD_WIDTH_IN_BLOCKS * BLOCK_SIZE },
        BOARD_HEIGHT{ BOARD_HEIGHT_IN_BLOCKS * BLOCK_SIZE },

        BOARD_POSITION_X{ (WINDOW_WIDTH - BOARD_WIDTH) / 2 },
        BOARD_POSITION_Y{ (WINDOW_HEIGHT - BOARD_HEIGHT) / 2 },

        TIME_DELAY{ 5 },
        PIECE_KINDS{ 7 },
        PIECE_ROTATIONS{ 4 },
        PIECE_INITIAL_SHIFT_X{ BLOCK_SIZE * 4 },
        FPS{ 60 },
        FRAME_DELAY{ 400 / FPS },

        SCORE_DIGIT_HEIGHT{ BLOCK_SIZE },
        SCORE_DIGIT_WIDTH{ BLOCK_SIZE },

        NUMBER_OF_SCORE_DIGITS{ 5 },
        DIGITS_GAP{ SCORE_DIGIT_WIDTH / 10 },

        INFO_BLOCK_POSITION_X{ BOARD_POSITION_X + BOARD_WIDTH },
        INFO_BLOCK_POSITION_Y{ BOARD_POSITION_Y + 2 * BLOCK_SIZE },

        INFO_BLOCK_WIDTH{ 7 * BLOCK_SIZE },
        INFO_BLOCK_HEIGHT{ 13 * BLOCK_SIZE },

        SCORE_HEIGHT{ SCORE_DIGIT_HEIGHT },
        SCORE_WIDTH{ (SCORE_DIGIT_WIDTH + DIGITS_GAP) * NUMBER_OF_SCORE_DIGITS - DIGITS_GAP },

        SCORE_POSITION_X{ INFO_BLOCK_POSITION_X + (INFO_BLOCK_WIDTH - SCORE_WIDTH) / 2 },
        SCORE_POSITION_Y{ INFO_BLOCK_POSITION_Y + 11 * BLOCK_SIZE / 4 },

        NEXT_PIECE_POSITION_Y{ INFO_BLOCK_POSITION_Y + 8 * BLOCK_SIZE },

        GAME_OVER_MESSAGE_WIDTH{ 25 * BLOCK_SIZE },
        GAME_OVER_MESSAGE_HEIGHT{ GAME_OVER_MESSAGE_WIDTH / 5 },

        START_AGAIN_MESSAGE_WIDTH{ GAME_OVER_MESSAGE_WIDTH / 2 },
        START_AGAIN_MESSAGE_HEIGHT{ GAME_OVER_MESSAGE_HEIGHT / 5 },

        BACKGROUND_RECT_WIDTH{ WINDOW_WIDTH },
        BACKGROUND_RECT_HEIGHT
        {
            START_AGAIN_MESSAGE_HEIGHT + GAME_OVER_MESSAGE_HEIGHT + 2 * BLOCK_SIZE
        },

        SCORE_ADDITION{ 9 },
        SCORE_MAX_VALUE{ 99999 },
        MAIN_FONT_SIZE{ 24 },
        SCENE_FONT_SIZE{ 24 },
        BUTTON_HEIGHT{ 140 },
        BUTTON_WIDTH{ 240 };

    const char* const GAME_WINDOW_NAME{ "Tetris" };
    
    const char* const FONT_FILE_PATH{ "./Fonts/FFFFORWA.TTF" };

    const char* const BLOCK_TEXTURE_FILE_PATH{ "./Textures/block_texture.png" };
    const char* const BACKGROUND_TEXTURE_FILE_PATH{ "./Textures/background_image.jpg" };
    const char* const BOARD_TEXTURE_FILE_PATH{ "./Textures/board_texture.png" };
    const char* const INFO_BLOCK_TEXTURE_FILE_PATH{ "./Textures/info_block_texture.png" };

    const SDL_Color
        BUTTON_FONT_COLOR{ 255, 0, 0 },
        MAIN_FONT_COLOR{ 255, 0, 0 };

    const SDL_Rect GAME_OVER_MESSAGE_RECTANGLE
    {
        (WINDOW_WIDTH - GAME_OVER_MESSAGE_WIDTH) / 2, 
        (WINDOW_HEIGHT - BACKGROUND_RECT_HEIGHT) / 2 + BLOCK_SIZE,
        GAME_OVER_MESSAGE_WIDTH,
        GAME_OVER_MESSAGE_HEIGHT
    };

    const SDL_Rect START_AGAIN_MESSAGE_RECTANGLE
    {
        (WINDOW_WIDTH - START_AGAIN_MESSAGE_WIDTH) / 2,
        (WINDOW_HEIGHT - BACKGROUND_RECT_HEIGHT) / 2 + GAME_OVER_MESSAGE_HEIGHT + BLOCK_SIZE,
        START_AGAIN_MESSAGE_WIDTH, 
        START_AGAIN_MESSAGE_HEIGHT
    };

    const SDL_Rect BACKGROUND_RECTANGLE
    {
        0,
        (WINDOW_HEIGHT - BACKGROUND_RECT_HEIGHT) / 2,
        BACKGROUND_RECT_WIDTH,
        BACKGROUND_RECT_HEIGHT
    };

    const SDL_Rect INFO_BLOCK_RECT
    {
        INFO_BLOCK_POSITION_X,
        INFO_BLOCK_POSITION_Y,
        INFO_BLOCK_WIDTH,
        INFO_BLOCK_HEIGHT
    };

    const SDL_Rect BOARD_RECT
    {
        BOARD_POSITION_X + BLOCK_SIZE,
        BOARD_POSITION_Y + 2 * BLOCK_SIZE,
        BOARD_WIDTH - 2 * BLOCK_SIZE,
        BOARD_HEIGHT - 3 * BLOCK_SIZE
    };

    const POINT MENU_BUTTON_POINT
    {
        (WINDOW_WIDTH - BUTTON_WIDTH) / 2,
        (WINDOW_HEIGHT - BUTTON_HEIGHT) / 2
    };

    const std::vector<SDL_Rect> SCORE_MESSAGE_RECTANGLES
    {
        {
            SCORE_POSITION_X,
            SCORE_POSITION_Y,
            SCORE_DIGIT_WIDTH,
            SCORE_DIGIT_HEIGHT
        },
        {
            SCORE_POSITION_X + SCORE_DIGIT_WIDTH + DIGITS_GAP,
            SCORE_POSITION_Y,
            SCORE_DIGIT_WIDTH,
            SCORE_DIGIT_HEIGHT
        },
        {
            SCORE_POSITION_X + 2 * (SCORE_DIGIT_WIDTH + DIGITS_GAP),
            SCORE_POSITION_Y,
            SCORE_DIGIT_WIDTH,
            SCORE_DIGIT_HEIGHT
        },
        {
            SCORE_POSITION_X + 3 * (SCORE_DIGIT_WIDTH + DIGITS_GAP),
            SCORE_POSITION_Y,
            SCORE_DIGIT_WIDTH,
            SCORE_DIGIT_HEIGHT
        },
        {
            SCORE_POSITION_X + 4 * (SCORE_DIGIT_WIDTH + DIGITS_GAP),
            SCORE_POSITION_Y,
            SCORE_DIGIT_WIDTH,
            SCORE_DIGIT_HEIGHT
        }
    };

    enum class Color {
        red,
        green,
        blue,
        black,
        transparentBlack,
        white
    };

    enum class FigureKind
    {
        Square = 0,
        I = 1,
        L = 2,
        J = 3,
        N = 4,
        N_mirrored = 5,
        T = 6
    };

    const FigureContainer Figures
    {
        {
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            }
        },
        {
            {
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 0}
            },
            {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 0}
            },
            {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0}
            }
        },
        {
            {
                {1, 0},
                {1, 0},
                {1, 1}
            },
            {
                {1, 1, 1},
                {1, 0, 0}
            },
            {
                {1, 1},
                {0, 1},
                {0, 1},
            },
            {
                {0, 0, 1},
                {1, 1, 1}
            }
        },
        {
            {
                {0, 1},
                {0, 1},
                {1, 1}
            },
            {
                {1, 0, 0},
                {1, 1, 1}
            },
            {
                {1, 1},
                {1, 0},
                {1, 0},
            },
            {
                {1, 1, 1},
                {0, 0, 1}
            }
        },
        {
            {
                {1, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 1},
                {1, 1},
                {1, 0}
            },
            {
                {1, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 1},
                {1, 1},
                {1, 0}
            },
        },
        {
            {
                {0, 1, 1},
                {1, 1, 0}
            },
            {
                {1, 0},
                {1, 1},
                {0, 1}
            },
            {
                {0, 1, 1},
                {1, 1, 0}
            },
            {
                {1, 0},
                {1, 1},
                {0, 1}
            },
        },
        {
            {
                {0, 1, 0},
                {1, 1, 1}
            },
            {
                {1, 0},
                {1, 1},
                {1, 0}
            },
            {
                {1, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 1},
                {1, 1},
                {0, 1}
            }
        }
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

    enum class GameState
    {
        MenuMode,
        Inactive,
        Running,
        Paused,
        GameOver
    };

    enum class PieceMovement
    {
        None,
        Left,
        Right,
        Rotation,
        SpeedUp,
    };

    struct PieceRotation 
    {
        int pieceShift{};
        bool pieceCanRotate{};
        int nextRotation{};
    };

    const Color BACKGROUND_COLOR{ Color::black };
}
