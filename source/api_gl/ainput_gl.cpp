#include "asharedh.hpp"
#include "asharedapi.hpp" // Glew & OpenGL
#include "ainput_gl.hpp"

// set static reference of g_input to the GL input
AGN::AInputGL inputGL = AGN::AInputGL();
AGN::IAInput& g_input = inputGL;

AGN::AInputGL::AInputGL()
	: m_mouseX(0)
	, m_mouseY(0)
{
	// initialize everything to false
	memset(m_heldChange, false, sizeof(m_heldChange));
	memset(m_held, false, sizeof(m_held));
	memset(m_mouse, false, sizeof(m_mouse));
	memset(m_click, false, sizeof(m_click));
}

AGN::SCANCODE AGN::AInputGL::getAGNScanCode(SDL_Scancode a_sdlkey)
{
	return (SCANCODE)a_sdlkey;
}

AGN::AGN_MOUSECODE AGN::AInputGL::getAGNMouse(uint8_t a_sdlMouse)
{
	return (AGN_MOUSECODE)a_sdlMouse;
}

void AGN::AInputGL::registerMouseClick(AGN::AGN_MOUSECODE a_mouseButton)
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

void AGN::AInputGL::registerMouseHold(AGN::AGN_MOUSECODE a_mouseButton, bool a_held)
{
	if (m_mouse[(int)a_mouseButton] != a_held)
	{
		m_click[(int)a_mouseButton] = true;
	}
	m_mouse[(int)a_mouseButton] = a_held;
}

void AGN::AInputGL::registerHold(AGN::SCANCODE a_key, bool a_held)
{
	if (m_held[(int)a_key] != a_held)
	{
		m_held[(int)a_key] = a_held;
		m_heldChange[(int)a_key] = true;
	}

	// TODO: if down, add to input string of characters (for inputfields and such)
	//if (m_doTrackInput && a_held) onInputCharacter(a_key);
}

void AGN::AInputGL::registerFrameCompletion()
{
	// set all heldchange back to false
	memset(m_heldChange, false, sizeof(m_heldChange));
	m_scrollAmount = 0;
}

void AGN::AInputGL::registerMouseMotion(int a_mouseX, int a_mouseY)
{
	m_oldMouseX = m_mouseX;
	m_mouseX = a_mouseX;

	m_oldMouseY = m_mouseY;
	m_mouseY = a_mouseY;

	for (IAInputMouseMotionHandler* motionHandler : m_mouseMotionHandlers)
	{
		motionHandler->onMouseMotion(m_mouseX, m_mouseY);
	}
}

bool AGN::AInputGL::getKeyDown(AGN::SCANCODE a_key)
{
	const bool retVal = m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::AInputGL::getKeyUp(AGN::SCANCODE a_key)
{
	const bool retVal = !m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::AInputGL::getKey(AGN::SCANCODE a_key)
{
	return m_held[(int)a_key];
}

bool AGN::AInputGL::getMouseDown(AGN::AGN_MOUSECODE a_mouseButton)
{
	if (m_click[(int)a_mouseButton] && m_mouse[(int)a_mouseButton])
	{
		m_click[(int)a_mouseButton] = false;
		return true;
	}
	else return false;
}

bool AGN::AInputGL::getMouseUp(AGN::AGN_MOUSECODE a_mouseButton)
{
	if (m_click[(int)a_mouseButton] && !m_mouse[(int)a_mouseButton])
	{
		m_click[(int)a_mouseButton] = false;
		return true;
	}
	else return false;
}

bool AGN::AInputGL::getMouse(AGN::AGN_MOUSECODE a_mouseButton)
{
	uint32_t mouseState = SDL_GetMouseState(NULL, NULL)&SDL_BUTTON((uint8_t)a_mouseButton);
	uint32_t buttonState = SDL_BUTTON((uint32_t)a_mouseButton);
	return (mouseState == buttonState);
}

bool AGN::AInputGL::getCapslock()
{
	return bool((SDL_GetModState() & KMOD_CAPS) == 1);
}

void AGN::AInputGL::addMouseMotionHandler(AGN::IAInputMouseMotionHandler* a_motionHandler)
{
	m_mouseMotionHandlers.push_back(a_motionHandler);
}

void AGN::AInputGL::removeMouseMotionHandler(AGN::IAInputMouseMotionHandler* a_motionHandler)
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

void AGN::AInputGL::addMouseClickHandler(class IAInputMouseClickHandler* a_clickHandler)
{
	m_mouseClickHandlers.push_back(a_clickHandler);
}

void AGN::AInputGL::removeMouseClickHandler(class IAInputMouseClickHandler* a_clickHandler)
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
