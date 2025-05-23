#include "ResourceHandler.h"
#include "Resources.h"

namespace GameNamespace
{
	ResourceHandler::ResourceHandler() {}
	ResourceHandler::~ResourceHandler() {}

	void ResourceHandler::LoadFonts(SDL_Renderer* renderer, int windowWidth)
	{
		fonts.clear();

		for (int i{}; i < FONTS_AMOUNT; i++)
		{
			fonts.push_back(std::unique_ptr<TTF_Font, SDLFontDestroyer>(
				LoadFont(renderer, FONT_SIZES[i], i, windowWidth)
			));
		}
	}

	void ResourceHandler::LoadTextures(SDL_Renderer* renderer)
	{
		for (int i{}; i < TEXTURES_AMOUNT; i++)
		{
			textures.push_back(std::unique_ptr<SDL_Texture, SDLTextureDestroyer>(
				LoadTexture(renderer, TEXTURES_FILE_PATHS[i])
			));
		}
	}

	TTF_Font* ResourceHandler::GetFont(Font font) const
	{
		return fonts[(int)font].get();
	}

	SDL_Texture* ResourceHandler::GetTexture(Texture texture) const
	{
		return textures[(int)texture].get();
	}

	SDL_Texture* ResourceHandler::LoadTexture(SDL_Renderer* renderer, const char* textureFilePath)
	{
		SDL_Surface* surface
		{
			IMG_Load(textureFilePath)
		};

		if (surface == NULL)
		{
			throw SurfaceNullReference();
		}

		SDL_Texture* texture
		{
			SDL_CreateTextureFromSurface(renderer, surface)
		};

		if (texture == NULL)
		{
			throw TextureNullReference();
		}

		SDL_FreeSurface(surface);

		return texture;
	}

	TTF_Font* ResourceHandler::LoadFont(SDL_Renderer* renderer, int fontSize, int fontIndex, int windowWidth)
	{
		TTF_Font* font{ TTF_OpenFont(FONT_FILE_PATH, CalcRelativeFontSize(fontSize, fontIndex, windowWidth)) };

		if (font == NULL)
		{
			throw FontNullReference();
		}

		return font;
	}

	unsigned ResourceHandler::CalcRelativeFontSize(unsigned fontSize, int fontIndex, int windowWidth) const
	{
		unsigned scale = windowWidth * FONT_SCALERS[fontIndex];

		return fontSize + scale;
	}
}
