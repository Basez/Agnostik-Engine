#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "input_gl.hpp"

#include <SDL/SDL.h>
// set static reference of g_input to the GL input
AGN::InputGL inputGL = AGN::InputGL();
AGN::IInput& g_input = inputGL;

AGN::InputGL::InputGL()
	: m_mouseX(0)
	, m_mouseY(0)
{
	// initialize everything to false
	memset(m_heldChange, false, sizeof(m_heldChange));
	memset(m_held, false, sizeof(m_held));
	memset(m_mouse, false, sizeof(m_mouse));
	memset(m_click, false, sizeof(m_click));
}

AGN::InputGL::~InputGL()
{
	// TODO: change to cleanup function as its a static class
	//printf("TODO: InputGL::~InputGL()");
}

AGN::ESCANCODE AGN::InputGL::getAGNScanCode(uint32_t a_sdlkey)
{
	return (ESCANCODE)a_sdlkey;
}

AGN::EMOUSECODE AGN::InputGL::getAGNMouse(uint8_t a_sdlMouse)
{
	return (EMOUSECODE)a_sdlMouse;
}

void AGN::InputGL::registerMouseClick(AGN::EMOUSECODE a_mouseButton)
{
	// TODO: Implement click callback system for UI's
	/*
	if (a_mouseButton == 1)
	{
		for (unsigned int i = 0; i < m_clickCallbacks.size(); i++)
		{
			(*m_clickCallbacks[i])(m_mouseX, m_mouseY);
		}
	}*/
}

void AGN::InputGL::registerMouseHold(AGN::EMOUSECODE a_mouseButton, bool a_held)
{
	if (m_mouse[(int)a_mouseButton] != a_held)
	{
		m_click[(int)a_mouseButton] = true;
	}
	m_mouse[(int)a_mouseButton] = a_held;
}

void AGN::InputGL::registerHold(AGN::ESCANCODE a_key, bool a_held)
{
	if (m_held[(int)a_key] != a_held)
	{
		m_held[(int)a_key] = a_held;
		m_heldChange[(int)a_key] = true;
	}

	// TODO: if down, add to input string of characters (for inputfields and such)
	//if (m_doTrackInput && a_held) onInputCharacter(a_key);
}

void AGN::InputGL::registerFrameCompletion()
{
	// set all heldchange back to false
	memset(m_heldChange, false, sizeof(m_heldChange));
	m_scrollAmount = 0;
}

void AGN::InputGL::registerMouseMotion(int a_mouseX, int a_mouseY)
{
	m_oldMouseX = m_mouseX;
	m_mouseX = a_mouseX;

	m_oldMouseY = m_mouseY;
	m_mouseY = a_mouseY;

	for (IInputMouseMotionHandler* motionHandler : m_mouseMotionHandlers)
	{
		motionHandler->onMouseMotion(m_mouseX, m_mouseY);
	}
}

bool AGN::InputGL::getKeyDown(AGN::ESCANCODE a_key)
{
	const bool retVal = m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::InputGL::getKeyUp(AGN::ESCANCODE a_key)
{
	const bool retVal = !m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::InputGL::getKey(AGN::ESCANCODE a_key)
{
	return m_held[(int)a_key];
}

bool AGN::InputGL::getMouseDown(AGN::EMOUSECODE a_mouseButton)
{
	if (m_click[(int)a_mouseButton] && m_mouse[(int)a_mouseButton])
	{
		m_click[(int)a_mouseButton] = false;
		return true;
	}
	else return false;
}

bool AGN::InputGL::getMouseUp(AGN::EMOUSECODE a_mouseButton)
{
	if (m_click[(int)a_mouseButton] && !m_mouse[(int)a_mouseButton])
	{
		m_click[(int)a_mouseButton] = false;
		return true;
	}
	else return false;
}

bool AGN::InputGL::getMouse(AGN::EMOUSECODE a_mouseButton)
{
	uint32_t mouseState = SDL_GetMouseState(NULL, NULL)&SDL_BUTTON((uint8_t)a_mouseButton);
	uint32_t buttonState = SDL_BUTTON((uint32_t)a_mouseButton);
	return (mouseState == buttonState);
}

bool AGN::InputGL::getCapslock()
{
	return bool((SDL_GetModState() & KMOD_CAPS) == 1);
}

void AGN::InputGL::addMouseMotionHandler(AGN::IInputMouseMotionHandler* a_motionHandler)
{
	m_mouseMotionHandlers.push_back(a_motionHandler);
}

void AGN::InputGL::removeMouseMotionHandler(AGN::IInputMouseMotionHandler* a_motionHandler)
{
	for (unsigned int i = 0; i < m_mouseMotionHandlers.size(); i++)
	{
		if (m_mouseMotionHandlers[i] == a_motionHandler)
		{
			m_mouseMotionHandlers.erase(m_mouseMotionHandlers.begin() + i);
			return;
		}
	}
}

void AGN::InputGL::addMouseClickHandler(class IInputMouseClickHandler* a_clickHandler)
{
	m_mouseClickHandlers.push_back(a_clickHandler);
}

void AGN::InputGL::removeMouseClickHandler(class IInputMouseClickHandler* a_clickHandler)
{
	for (unsigned int i = 0; i < m_mouseClickHandlers.size(); i++)
	{
		if (m_mouseClickHandlers[i] == a_clickHandler)
		{
			m_mouseClickHandlers.erase(m_mouseClickHandlers.begin() + i);
			return;
		}
	}
}
