#pragma once
#include <vector>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_main.h>
#include "GameExceptions.h"
#include "Layout.h"
#include "Constants.h"

namespace GameNamespace
{
	struct SDLTextureDestroyer
	{
		void operator()(SDL_Texture* texture) const
		{
			SDL_DestroyTexture(texture);
		}
	};

	struct SDLFontDestroyer
	{
		void operator()(TTF_Font* font) const
		{
			TTF_CloseFont(font);
		}
	};

	class ResourceHandler
	{
	public:

		ResourceHandler();
		~ResourceHandler();

		void LoadFonts(SDL_Renderer* renderer);
		void LoadTextures(SDL_Renderer* renderer);

		TTF_Font* GetFont(Font font) const;
		SDL_Texture* GetTexture(Texture texture) const;

	private:

		std::vector<std::unique_ptr<TTF_Font, SDLFontDestroyer>> fonts{};
		std::vector<std::unique_ptr<SDL_Texture, SDLTextureDestroyer>> textures{};

		SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* textureFilePath);
		TTF_Font* LoadFont(SDL_Renderer* renderer, int fontSize, int fontIndex);

		unsigned CalcRelativeFontSize(unsigned fontSize, int fontIndex);
	};
}
