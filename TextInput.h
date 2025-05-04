#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Constants.h"
#include "GameExceptions.h"
#include <SDL_timer.h>

namespace GameNamespace
{
    class TextInput {
    public:
        TextInput(
            SDL_Renderer* renderer,
            SDL_Texture* texture,
            TTF_Font* font,
            SDL_Rect rect,
            SDL_Color textColor,
            SDL_Color caretColor,
            int textMaxLenght);
        ~TextInput();

        void HandleMouseLeftClick(POINT pressPoint);
        void HandleTextInput(const char* textInput);
        void HandleKeyDown(SDL_Keycode keyCode);
        void Render(SDL_Renderer* renderer);
        const std::string& GetText() const;

    private:
        SDL_Texture* texture{};
        TTF_Font* font{};
        SDL_Color textColor{};
        SDL_Color caretColor{};
        SDL_Rect rect{};
        std::string text{};
        bool isActive{};
        int caretPosition{};
        Uint32 lastBlinkTime{};
        bool caretVisible{ true };
        int scrollOffset{};
        int textMaxLength{};

        void RenderCaret(SDL_Renderer* renderer);
        void ScrollTextIfNeeded();
        bool IsValidCharacter(char c) const;
    };
};
