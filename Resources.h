#pragma once

#include "Constants.h"

namespace GameNamespace
{
    const char* const FONT_FILE_PATH{ "./Fonts/FFFFORWA.TTF" };

    const char* TEXTURES_FILE_PATHS[TEXTURES_AMOUNT]
    {
        "./Textures/block_texture.png",
        "./Textures/background_image.jpg",
        "./Textures/board_texture.png",
        "./Textures/info_block_texture.png",
        "./Textures/text_input_texture.png"
    };

    const int FONT_SIZES[FONTS_AMOUNT]
    {
        6, // Main
        1, // Scene
        19, // Button
        0, // Score
        8 // TextInput
    };

    const double FONT_SCALERS[FONTS_AMOUNT]
    {
        0.067857,
        0.014285,
        0.026785,
        0.025571,
        0.014285
    };
}
