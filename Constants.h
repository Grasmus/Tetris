#pragma once
#include <SDL_ttf.h>
#include <vector>
#include <wtypes.h>

namespace GameNamespace
{
    typedef std::vector<std::vector<std::vector<std::vector<int>>>> FigureContainer;
    typedef std::vector<std::vector<int>> Figure;

    const int
        TIME_DELAY{ 5 },
        PIECE_KINDS{ 7 },
        PIECE_ROTATIONS{ 4 },
        FPS{ 60 },
        FRAME_DELAY{ 400 / FPS },

        SCORE_ADDITION{ 9 },
        SCORE_MAX_VALUE{ 99999 },

        TEXT_INPUT_CARRET_BLINK_MS{ 500 },
        TEXT_INPUT_CARRET_WIDTH{ 2 };

    const float
        TEXT_INPUT_RECT_OFFSET_COEF{ 0.1 },
        TEXT_INPUT_HEIGHT_ELEVATION{ 0.25 },
        TEXT_INPUT_ELEVATION_COEF{ 1 - TEXT_INPUT_HEIGHT_ELEVATION },
        TEXT_INPUT_CLIP_RECT_X_OFFSET{ 0.1 },
        TEXT_INPUT_CLIP_RECT_WIDTH_OFFSET{ 0.2 },
        TEXT_INPUT_CARRET_RECT_Y_OFFSET{ 0.15 },
        TEXT_INPUT_CARRET_RECT_HEIGHT_COEF{ 0.3 },
        TEXT_INPUT_CARRET_INIT_OFFSET_COEF{ 0.1 };

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
        Main,
        Scene,
        Button,
        Score,
        TextInput,

        AMOUNT // this value should always be last in enum
    };

    const int FONTS_AMOUNT{ (int)Font::AMOUNT };

    enum class Texture
    {
        Block,
        Backgroun,
        Board,
        InfoBlock,
        TextInput,

        AMOUNT // this value should always be last in enum
    };

    const int TEXTURES_AMOUNT{ (int)Texture::AMOUNT };

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
