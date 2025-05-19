#pragma once
#include "Constants.h"

namespace GameNamespace
{
    const int
        DEFAULT_WINDOW_WIDTH{ 1920 },
        DEFAULT_WINDOW_HEIGHT{ 1080 },

        BOARD_WIDTH_IN_BLOCKS{ 11 },
        BOARD_HEIGHT_IN_BLOCKS{ 23 },

        INFO_BLOCK_WIDTH_IN_BLOCKS{ 7 },
        INFO_BLOCK_HEIGHT_IN_BLOCKS{ 13 },

        NUMBER_OF_SCORE_DIGITS{ 5 },

        BLOCK_SIZE_SCALER{ 48 },

        BACKGROUND_RECT_HEIGHT_IN_BLOCKS{ 10 },

        SAVE_SETTINGS_BUTTON_Y_OFFSET_SCALER{ 5 };

    const Color
        MAIN_FONT_COLOR{ Color::Red };

    const SDL_Point RESOLUTION_SETTING_POSITION_SCALERS{ 3, 7 };

    class Layout
    {
    public:
        Layout(int width, int height);
        ~Layout();

        int GetWindowWidth() const;
        int GetWindowHeight() const;
        int GetBlockSize() const;
        int GetPieceInitialShiftX() const;
        int GetScoreYOffset() const;
        int GetNextPiecePositionY() const;

        SDL_Rect GetBoardRect() const;
        SDL_Rect GetDrawBoardRect() const;
        SDL_Rect GetInfoBlockRect() const;
        SDL_Rect GetBackgroundRect() const;

        void CalculateParameters(int width, int height);

    private:
        int windowWidth{},
            windowHeight{},
            blockSize{},
            pieceInitialShiftX{},
            scoreYOffset{},
            nextPiecePositionY{};

        SDL_Rect boardRect{},
            drawBoardRect{},
            infoBlockRect{},
            backgroundRect{};

        void CalculateParameters();
    };
}
