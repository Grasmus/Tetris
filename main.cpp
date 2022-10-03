#include "Game.h"
#include "GameExceptions.h"
#include <memory>

using GameNamespace::Game;

int main(int argc, char* argv[])
{
    Uint32 frameStart{};
    int frameTime{};

    std::unique_ptr<Game> game{ new Game() };

    try
    {
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
        printf(exception.what());
    }
    catch (...)
    {
        printf("Something went wrong :(");
    }

    return 0;
}
