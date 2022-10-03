#pragma once
#include <exception>

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

struct MessageNullReference : public std::exception {
	const char* what() const throw () {
		return "Message null reference";
	}
};

struct SDLInitException : public std::exception {
	const char* what() const throw () {
		return "SLD_Init exception";
	}
};
