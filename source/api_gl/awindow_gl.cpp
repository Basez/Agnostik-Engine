#include "asharedh.hpp"
#include "asharedapi.hpp" // Glew & OpenGL
#include "awindow_gl.hpp"
#include "aosutils.hpp"

AGN::AWindowGL::AWindowGL(glm::ivec2 a_dimentions)
	: m_dimentions(a_dimentions)
	, m_mouseOnScreen(false)
	, m_keyboardFocus(false)
	, m_minimized(false)
{
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	std::string windowName;

#ifdef AGN_DEBUG
	windowName = std::string("Agnostik OpenGL - DEBUG");
#elif AGN_RELEASE
	windowName = std::string("Agnostik OpenGL - RELEASE");
#else
	windowName = AGN::AOSUtils::getExecutableName();
#endif

	m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, a_dimentions.x, a_dimentions.y, flags);

	if (!m_sdlWindow)
	{
		SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(), m_sdlWindow);
	}

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

void AGN::AWindowGL::warpCursor(glm::ivec2 a_screenPosition)
{
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_WarpMouseInWindow(m_sdlWindow, a_screenPosition.x, a_screenPosition.y);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
}

void AGN::AWindowGL::showCursor(bool a_shown)
{
	SDL_ShowCursor(a_shown);
}

void AGN::AWindowGL::onWindowEvent(SDL_Event a_event)
{
	switch (a_event.window.event)
	{
	case SDL_WINDOWEVENT_RESIZED:
		m_dimentions.x = a_event.window.data1;
		m_dimentions.y = a_event.window.data2;
		glViewport(0, 0, m_dimentions.x, m_dimentions.y);
		break;

	case SDL_WINDOWEVENT_ENTER:
		m_mouseOnScreen = true;
		break;

	case SDL_WINDOWEVENT_LEAVE:
		m_mouseOnScreen = false;
		break;

	case SDL_WINDOWEVENT_FOCUS_GAINED:
		m_keyboardFocus = true;
		break;

	case SDL_WINDOWEVENT_FOCUS_LOST:
		m_keyboardFocus = false;
		break;

	case SDL_WINDOWEVENT_MINIMIZED:
		m_minimized = true;
		break;

	case SDL_WINDOWEVENT_RESTORED:	// triggered when window is "unminimized"
		m_minimized = false;
		break;
	}
}