#include "TextInput.h"
#include <stdexcept>
#include <SDL_timer.h>

TextInput::TextInput(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, SDL_Rect rect)
    : texture(texture), font(font), rect(rect), isActive(false), caretPosition(0), lastBlinkTime(0), caretVisible(true), scrollOffset(0) {
    if (!texture || !font) {
        throw std::invalid_argument("Texture or font cannot be null.");
    }
}

TextInput::~TextInput() {
    if (isActive) {
        SDL_StopTextInput();
    }
}

void TextInput::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            isActive = true;
            SDL_StartTextInput();
        } else {
            isActive = false;
            SDL_StopTextInput();
        }
    }

    if (event.type == SDL_TEXTINPUT && isActive) {
        for (char c : std::string(event.text.text)) {
            if (IsValidCharacter(c)) {
                text.insert(caretPosition++, 1, c);
                ScrollTextIfNeeded();
            }
        }
    } else if (event.type == SDL_KEYDOWN && isActive) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && caretPosition > 0) {
            text.erase(--caretPosition, 1);
            ScrollTextIfNeeded();
        } else if (event.key.keysym.sym == SDLK_LEFT && caretPosition > 0) {
            caretPosition--;
            ScrollTextIfNeeded();
        } else if (event.key.keysym.sym == SDLK_RIGHT && caretPosition < text.size()) {
            caretPosition++;
            ScrollTextIfNeeded();
        }
    }
}

void TextInput::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    if (!text.empty()) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), {0, 0, 0, 255});
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int textWidth, textHeight;
        TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);

        SDL_Rect textRect = {rect.x + 10 + scrollOffset, rect.y + (rect.h - textHeight) / 2, textWidth, textHeight};

        SDL_Rect clipRect = {rect.x + 10, rect.y, rect.w - 20, rect.h};
        SDL_RenderSetClipRect(renderer, &clipRect);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_RenderSetClipRect(renderer, NULL);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    RenderCaret(renderer);
}

const std::string& TextInput::GetText() const {
    return text;
}

void TextInput::RenderCaret(SDL_Renderer* renderer) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastBlinkTime > 500) {
        caretVisible = !caretVisible;
        lastBlinkTime = currentTime;
    }

    if (caretVisible && isActive) {
        int textWidth = 0;
        if (caretPosition > 0) {
            std::string textBeforeCaret = text.substr(0, caretPosition);
            TTF_SizeText(font, textBeforeCaret.c_str(), &textWidth, nullptr);
        }

        int caretX = rect.x + 10 + textWidth + scrollOffset;
        SDL_Rect caretRect = {caretX, rect.y + 5, 2, rect.h - 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &caretRect);
    }
}

void TextInput::ScrollTextIfNeeded() {
    int textWidth = 0;
    if (caretPosition > 0) {
        std::string textBeforeCaret = text.substr(0, caretPosition);
        TTF_SizeText(font, textBeforeCaret.c_str(), &textWidth, nullptr);
    }

    if (textWidth + scrollOffset > rect.w - 20) {
        scrollOffset = rect.w - 20 - textWidth;
    } else if (textWidth + scrollOffset < 10) {
        scrollOffset = 10 - textWidth;
    }
}

bool TextInput::IsValidCharacter(char c) const {
    return (isalnum(static_cast<unsigned char>(c)) || c == '_');
}
