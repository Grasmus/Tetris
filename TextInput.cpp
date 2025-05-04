#include "TextInput.h"

namespace GameNamespace
{
    TextInput::TextInput(
        SDL_Renderer* renderer,
        SDL_Texture* texture,
        TTF_Font* font,
        SDL_Rect rect,
        SDL_Color textColor,
        SDL_Color caretColor
    ): 
        rect(rect),
        isActive(false),
        textColor(textColor),
        caretPosition(0),
        caretColor(caretColor),
        lastBlinkTime(0),
        caretVisible(true),
        scrollOffset(0)
    {
        if (texture == NULL)
        {
            throw TextureNullReference();
        }

        if (font == NULL)
        {
            throw FontNullReference();
        }

        this->texture = texture;
        this->font = font;
    }

    TextInput::~TextInput()
    {
        if (isActive)
        {
            SDL_StopTextInput();
        }

        SDL_DestroyTexture(texture);
    }

    void TextInput::HandleMouseLeftClick(POINT pressPoint)
    {
        if (pressPoint.x >= rect.x && pressPoint.x <= rect.x + rect.w &&
            pressPoint.y >= rect.y && pressPoint.y <= rect.y + rect.h)
        {
            isActive = true;

            SDL_StartTextInput();
        }
        else
        {
            isActive = false;

            SDL_StopTextInput();
        }
    }

    void TextInput::HandleTextInput(const char* textInput)
    {
        if (isActive)
        {
            for (char c : std::string(textInput))
            {
                if (IsValidCharacter(c))
                {
                    text.insert(caretPosition++, 1, c);

                    ScrollTextIfNeeded();
                }
            }
        }
    }

    void TextInput::HandleKeyDown(SDL_Keycode keyCode)
    {
        if (isActive)
        {
            switch (keyCode)
            {
            case SDLK_BACKSPACE:
                if (caretPosition > 0)
                {
                    text.erase(--caretPosition, 1);
                }

                break;

            case SDLK_LEFT:
                if (caretPosition > 0)
                {
                    caretPosition--;
                }

                break;

            case SDLK_RIGHT:
                if (caretPosition < text.size())
                {
                    caretPosition++;
                }

                break;
            }

            ScrollTextIfNeeded();
        }
    }

    void TextInput::Render(SDL_Renderer* renderer)
    {
        SDL_RenderCopy(renderer, texture, NULL, &rect);

        if (!text.empty())
        {
            SDL_Surface* textSurface{
                TTF_RenderText_Solid(font, text.c_str(), textColor)
            };

            SDL_Texture* textTexture{
                SDL_CreateTextureFromSurface(renderer, textSurface)
            };

            int textWidth{}, textHeight{};

            TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);

            SDL_Rect textRect{
                rect.x + scrollOffset + rect.w * TEXT_INPUT_RECT_OFFSET_COEF,
                rect.y - textHeight + rect.h * TEXT_INPUT_ELEVATION_COEF,
                textWidth,
                textHeight
            };

            SDL_Rect clipRect{
                rect.x + rect.w * TEXT_INPUT_CLIP_RECT_X_OFFSET,
                rect.y,
                rect.w - rect.w * TEXT_INPUT_CLIP_RECT_WIDTH_OFFSET,
                rect.h
            };

            SDL_RenderSetClipRect(renderer, &clipRect);

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_RenderSetClipRect(renderer, NULL);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

        RenderCaret(renderer);
    }

    const std::string& TextInput::GetText() const
    {
        return text;
    }

    void TextInput::RenderCaret(SDL_Renderer* renderer)
    {
        Uint32 currentTime{ SDL_GetTicks() };

        if (currentTime - lastBlinkTime > TEXT_INPUT_CARRET_BLINK_MS)
        {
            caretVisible = !caretVisible;
            lastBlinkTime = currentTime;
        }

        if (caretVisible && isActive)
        {
            int textWidth{}, textHeight{}, carretX{};

            if (caretPosition > 0)
            {
                std::string textBeforeCaret{ text.substr(0, caretPosition) };

                TTF_SizeText(font, textBeforeCaret.c_str(), &textWidth, &textHeight);

                carretX = rect.x + rect.w * TEXT_INPUT_RECT_OFFSET_COEF + textWidth + scrollOffset;
            }
            else
            {
                carretX = rect.x + rect.w * TEXT_INPUT_CARRET_INIT_OFFSET_COEF;
            }

            SDL_Rect caretRect{
                carretX,
                rect.y + rect.h * TEXT_INPUT_CARRET_RECT_Y_OFFSET,
                TEXT_INPUT_CARRET_WIDTH,
                rect.h - rect.h * TEXT_INPUT_CARRET_RECT_HEIGHT_COEF
            };

            SDL_SetRenderDrawColor(
                renderer, 
                caretColor.r, 
                caretColor.g, 
                caretColor.b, 
                caretColor.a
            );
            SDL_RenderFillRect(renderer, &caretRect);
        }
    }

    void TextInput::ScrollTextIfNeeded()
    {
        int textWidth{};

        if (caretPosition > 0)
        {
            std::string textBeforeCaret{ text.substr(0, caretPosition) };

            TTF_SizeText(font, textBeforeCaret.c_str(), &textWidth, nullptr);
        }

        int textPaddingRight = rect.w - rect.w * TEXT_INPUT_CLIP_RECT_WIDTH_OFFSET;

        if (textWidth > textPaddingRight)
        {
            scrollOffset = textPaddingRight - textWidth;
        }
        else
        {
            scrollOffset = 0;
        }
    }

    bool TextInput::IsValidCharacter(char c) const {
        return (isalnum(static_cast<unsigned char>(c)) || c == '_');
    }
};
