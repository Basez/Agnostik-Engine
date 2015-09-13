#include "asharedh.hpp"
#include "awindow_gl.hpp"

// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

AGN::AWindowGL::AWindowGL(glm::ivec2 a_dimentions)
	: m_dimentions(a_dimentions)
{
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	// TODO:
	//string appName = //FileUtils::getExecutableName();
	//string windowName;

	// TODO:
	//#ifdef NDEBUG
	//	windowName = appName + string(" - RELEASE");
	//#else
	//	windowName = appName + string(" - DEBUG");
	//#endif

	m_sdlWindow = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, a_dimentions.x, a_dimentions.y, flags);

	if (m_sdlWindow == nullptr)
		SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(), m_sdlWindow);

	glViewport(0, 0, a_dimentions.x, a_dimentions.y);
}

void AGN::AWindowGL::setTitle(const char* a_title)
{
	SDL_SetWindowTitle(m_sdlWindow, a_title);	
}

void AGN::AWindowGL::showMessageBox(const char* a_title, const char* a_message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, a_title, a_message, m_sdlWindow);
}

