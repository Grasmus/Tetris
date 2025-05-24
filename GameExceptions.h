#pragma once
#include <exception>

namespace GameNamespace
{
	struct GameAlreadyCreatedException : public std::exception {
		const char* what() const throw () {
			return "Object Game already exists!";
		}
	};

	struct WindowCreationException : public std::exception {
		const char* what() const throw () {
			return "Window hasn`t been created";
		}
	};

	struct RenderCreationException : public std::exception {
		const char* what() const throw () {
			return "Renderer hasn`t been created";
		}
	};

	struct FontNullReference : public std::exception {
		const char* what() const throw () {
			return "Font null reference";
		}
	};

	struct SurfaceNullReference : public std::exception {
		const char* what() const throw () {
			return "Surface null reference";
		}
	};

	struct TextureNullReference : public std::exception {
		const char* what() const throw () {
			return "Texture null reference";
		}
	};

	struct MessageNullReference : public std::exception {
		const char* what() const throw () {
			return "Message null reference";
		}
	};

	struct SDLInitException : public std::exception {
		const char* what() const throw () {
			return "SDL_Init failed";
		}
	};

	struct TTFInitException : public std::exception {
		const char* what() const throw () {
			return "TTF_Init failed";
		}
	};

	struct SetTextureAlphaModException : public std::exception {
		const char* what() const throw () {
			return "SDL_SetTextureAlphaMod failed";
		}
	};

	struct TTFSizeTextException : public std::exception {
		const char* what() const throw () {
			return "TTF_SizeText failed";
		}
	};

	struct SettingsFileNameException : public std::exception {
		const char* what() const throw () {
			return "Cannot open settings file";
		}
	};

	struct SettingNotFoundException : public std::exception {
		const char* what() const throw () {
			return "Invalid setting name";
		}
	};

	struct SettingsInvalidSyntaxException : public std::exception {
		const char* what() const throw () {
			return "Invalid settings syntax";
		}
	};

	struct SettingsInvalidTypeException : public std::exception {
		const char* what() const throw () {
			return "Invalid settings type";
		}
	};

	struct DatabaseOpenException : public std::exception {
		const char* what() const throw () {
			return "Cannot open database";
		}
	};

	struct DatabaseCreateTableException : public std::exception {
		const char* what() const throw () {
			return "Failed to create table";
		}
	};
};
