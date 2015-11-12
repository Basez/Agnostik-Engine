#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "window_gl.hpp"
#include "os_utils.hpp"

#include <SDL/SDL.h>
#include <GL/glew.h>

AGN::WindowGL::WindowGL(glm::ivec2 a_dimensions)
	: m_dimensions(a_dimensions)
	, m_mouseOnScreen(false)
	, m_keyboardFocus(false)
	, m_minimized(false)
	, m_isDirty(false)
{
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

#ifdef AGN_DEBUG
	m_windowName = std::string("Agnostik OpenGL - DEBUG");
#elif AGN_RELEASE
	m_windowName = std::string("Agnostik OpenGL - RELEASE");
#else
	m_windowName = AGN::OSUtils::getExecutableName();
#endif

	m_sdlWindow = SDL_CreateWindow(m_windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, a_dimensions.x, a_dimensions.y, flags);

	if (!m_sdlWindow)
	{
		SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(), m_sdlWindow);
	}

	glViewport(0, 0, a_dimensions.x, a_dimensions.y);
}

AGN::WindowGL::~WindowGL()
{
	SDL_DestroyWindow(m_sdlWindow);
}

void AGN::WindowGL::setTitle(const char* a_title)
{
	m_windowName = a_title;
	SDL_SetWindowTitle(m_sdlWindow, m_windowName.c_str());
}

void AGN::WindowGL::showMessageBox(const char* a_title, const char* a_message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, a_title, a_message, m_sdlWindow);
}

void AGN::WindowGL::warpCursor(glm::ivec2 a_screenPosition)
{
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_WarpMouseInWindow(m_sdlWindow, a_screenPosition.x, a_screenPosition.y);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
}

void AGN::WindowGL::showCursor(bool a_shown)
{
	SDL_ShowCursor(a_shown);
}

void AGN::WindowGL::onWindowEvent(SDL_Event& a_event)
{
	switch (a_event.window.event)
	{
	case SDL_WINDOWEVENT_RESIZED:
		m_isDirty = true;
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

void AGN::WindowGL::updateWindowState()
{
	SDL_GetWindowSize(m_sdlWindow, &m_dimensions.x, &m_dimensions.y);

	glViewport(0, 0, m_dimensions.x, m_dimensions.y);

	m_isDirty = false;

	g_log.info("Resized Window to: %ix%i", m_dimensions.x, m_dimensions.y);
}