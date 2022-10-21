#include "Game.h"
#include "GameExceptions.h"
#include <memory>
#include <string>

using GameNamespace::Game;

int main(int argc, char* argv[])
{
    Uint32 frameStart{};
    int frameTime{};

    try
    {
        std::unique_ptr<Game> game{ new Game() };

        while (game->Running())
        {
            frameStart = SDL_GetTicks();

            game->HandleEvents();
            game->Update();
            game->Render();
            game->AddFrame();

            frameTime = SDL_GetTicks() - frameStart;

            if (game->GetFrameDelay() > frameTime)
            {
                SDL_Delay(game->GetFrameDelay() - frameTime);
            }
        }
    }
    catch (std::exception& exception)
    {
        std::string errorMessage{ exception.what() };
        errorMessage.append(SDL_GetError());

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", errorMessage.c_str(), NULL);
        SDL_Quit();
    }
    catch (...)
    {
        std::string errorMessage{ "Something went wrong :(" };
        errorMessage.append(SDL_GetError());

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", errorMessage.c_str(), NULL);
        SDL_Quit();
    }

    return 0;
}
