#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <SDL_timer.h>
#include "GameExceptions.h"
#include "UIElement.h"
#include "Constants.h"

namespace GameNamespace
{
    class TextInput : public UIElement {
    public:
        TextInput(
            SDL_Rect rect,
            SDL_Texture* texture,
            TTF_Font* font,
            SDL_Color textColor,
            SDL_Color caretColor,
            int textMaxLenght,
            void (*onKeyDown)(std::string&)
        );
        ~TextInput();

        bool HandleMouseLeftClick(SDL_Point pressPoint);
        void HandleTextInput(const char* textInput);
        void HandleKeyDown(SDL_Keycode keyCode);
        void Render(SDL_Renderer* renderer);
        const std::string& GetText() const;

    private:
        SDL_Texture* texture{};
        TTF_Font* font{};
        SDL_Color textColor{};
        SDL_Color caretColor{};
        std::string text{};
        bool isActive{};
        int caretPosition{};
        Uint32 lastBlinkTime{};
        bool caretVisible{ true };
        int scrollOffset{};
        int textMaxLength{};

        void (*onKeyDown)(std::string&);

        void RenderCaret(SDL_Renderer* renderer);
        void ScrollTextIfNeeded();
        bool IsValidCharacter(char c) const;
    };
};
