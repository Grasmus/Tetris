#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TextInput {
public:
    TextInput(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, SDL_Rect rect);
    ~TextInput();

    void HandleEvent(const SDL_Event& event);
    void Render(SDL_Renderer* renderer);
    const std::string& GetText() const;

private:
    SDL_Texture* texture;
    TTF_Font* font;
    SDL_Rect rect;
    std::string text;
    bool isActive;
    int caretPosition;
    Uint32 lastBlinkTime;
    bool caretVisible;
    int scrollOffset;

    void RenderCaret(SDL_Renderer* renderer);
    void ScrollTextIfNeeded();
    bool IsValidCharacter(char c) const;
};
