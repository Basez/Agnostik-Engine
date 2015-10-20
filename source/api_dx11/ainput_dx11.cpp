#include "asharedh.hpp"
#include "ainput_dx11.hpp"
#include "dinput.h"

// set static reference of g_input to the DX11 input
AGN::AInputDX11 inputDX11 = AGN::AInputDX11();
AGN::IAInput& g_input = inputDX11;

static uint16_t s_dx11mappedAGNScancodes[static_cast<unsigned int>(AGN::SCANCODE::NUM_SCANCODES)];

static void mapScanCode(uint16_t a_dx11scancode, AGN::SCANCODE a_agnCode)
{
	//BX_CHECK(_sdl < BX_COUNTOF(s_translateKey), "Out of bounds %d.", _sdl);
	s_dx11mappedAGNScancodes[a_dx11scancode] = (uint8_t)a_agnCode;
}

AGN::AInputDX11::AInputDX11()
	: m_mouseX(0)
	, m_mouseY(0)
{
	// initialize everything to false
	memset(m_heldChange, false, sizeof(m_heldChange));
	memset(m_held, false, sizeof(m_held));
	memset(m_mouse, false, sizeof(m_mouse));
	memset(m_click, false, sizeof(m_click));

	// map keys
	memset(s_dx11mappedAGNScancodes, 0, static_cast<unsigned int>(AGN::SCANCODE::NUM_SCANCODES) * sizeof(uint16_t));
	mapScanCode(DIK_1, AGN::SCANCODE::TOPNUM_1);
	mapScanCode(DIK_2, AGN::SCANCODE::TOPNUM_2);
	mapScanCode(DIK_3, AGN::SCANCODE::TOPNUM_3);
	mapScanCode(DIK_4, AGN::SCANCODE::TOPNUM_4);
	mapScanCode(DIK_5, AGN::SCANCODE::TOPNUM_5);
	mapScanCode(DIK_6, AGN::SCANCODE::TOPNUM_6);
	mapScanCode(DIK_7, AGN::SCANCODE::TOPNUM_7);
	mapScanCode(DIK_8, AGN::SCANCODE::TOPNUM_8);
	mapScanCode(DIK_9, AGN::SCANCODE::TOPNUM_9);
	mapScanCode(DIK_0, AGN::SCANCODE::TOPNUM_0);
	mapScanCode(DIK_MINUS, AGN::SCANCODE::MINUS);
	mapScanCode(DIK_EQUALS, AGN::SCANCODE::EQUALS);
	mapScanCode(DIK_BACK, AGN::SCANCODE::BACKSPACE);
	mapScanCode(DIK_TAB, AGN::SCANCODE::TAB);
	mapScanCode(DIK_Q, AGN::SCANCODE::Q);
	mapScanCode(DIK_W, AGN::SCANCODE::W);
	mapScanCode(DIK_E, AGN::SCANCODE::E);
	mapScanCode(DIK_R, AGN::SCANCODE::R);
	mapScanCode(DIK_T, AGN::SCANCODE::T);
	mapScanCode(DIK_Y, AGN::SCANCODE::Y);
	mapScanCode(DIK_U, AGN::SCANCODE::U);
	mapScanCode(DIK_I, AGN::SCANCODE::I);
	mapScanCode(DIK_O, AGN::SCANCODE::O);
	mapScanCode(DIK_P, AGN::SCANCODE::P);
	mapScanCode(DIK_LBRACKET, AGN::SCANCODE::LEFTBRACKET);
	mapScanCode(DIK_RBRACKET, AGN::SCANCODE::RIGHTBRACKET);
	mapScanCode(DIK_RETURN, AGN::SCANCODE::RETURN);
	mapScanCode(DIK_LCONTROL, AGN::SCANCODE::LCTRL);
	mapScanCode(DIK_A, AGN::SCANCODE::A);
	mapScanCode(DIK_S, AGN::SCANCODE::S);
	mapScanCode(DIK_D, AGN::SCANCODE::D);
	mapScanCode(DIK_F, AGN::SCANCODE::F);
	mapScanCode(DIK_G, AGN::SCANCODE::G);
	mapScanCode(DIK_H, AGN::SCANCODE::H);
	mapScanCode(DIK_J, AGN::SCANCODE::J);
	mapScanCode(DIK_K, AGN::SCANCODE::K);
	mapScanCode(DIK_L, AGN::SCANCODE::L);
	mapScanCode(DIK_SEMICOLON, AGN::SCANCODE::SEMICOLON);
	mapScanCode(DIK_APOSTROPHE, AGN::SCANCODE::APOSTROPHE);
	mapScanCode(DIK_GRAVE, AGN::SCANCODE::GRAVE);
	mapScanCode(DIK_LSHIFT, AGN::SCANCODE::LSHIFT);
	mapScanCode(DIK_BACKSLASH, AGN::SCANCODE::BACKSLASH);
	mapScanCode(DIK_Z, AGN::SCANCODE::Z);
	mapScanCode(DIK_X, AGN::SCANCODE::X);
	mapScanCode(DIK_C, AGN::SCANCODE::C);
	mapScanCode(DIK_V, AGN::SCANCODE::V);
	mapScanCode(DIK_B, AGN::SCANCODE::B);
	mapScanCode(DIK_N, AGN::SCANCODE::N);
	mapScanCode(DIK_M, AGN::SCANCODE::M);
	mapScanCode(DIK_COMMA, AGN::SCANCODE::COMMA);
	mapScanCode(DIK_PERIOD, AGN::SCANCODE::PERIOD);
	mapScanCode(DIK_SLASH, AGN::SCANCODE::SLASH);
	mapScanCode(DIK_RSHIFT, AGN::SCANCODE::RSHIFT);
	mapScanCode(DIK_MULTIPLY, AGN::SCANCODE::KP_MULTIPLY);
	//mapScanCode(DIK_LMENU, AGN::SCANCODE::??);	// TODO
	mapScanCode(DIK_SPACE, AGN::SCANCODE::SPACE);
	//mapScanCode(DIK_CAPITAL, AGN::SCANCODE::??); // TODO
	mapScanCode(DIK_F1, AGN::SCANCODE::F1);
	mapScanCode(DIK_F2, AGN::SCANCODE::F2);
	mapScanCode(DIK_F3, AGN::SCANCODE::F3);
	mapScanCode(DIK_F4, AGN::SCANCODE::F4);
	mapScanCode(DIK_F5, AGN::SCANCODE::F5);
	mapScanCode(DIK_F6, AGN::SCANCODE::F6);
	mapScanCode(DIK_F7, AGN::SCANCODE::F7);
	mapScanCode(DIK_F8, AGN::SCANCODE::F8);
	mapScanCode(DIK_F9, AGN::SCANCODE::F9);
	mapScanCode(DIK_F10, AGN::SCANCODE::F10);
	mapScanCode(DIK_F11, AGN::SCANCODE::F11);
	mapScanCode(DIK_F12, AGN::SCANCODE::F12);
	mapScanCode(DIK_F13, AGN::SCANCODE::F13);
	mapScanCode(DIK_F14, AGN::SCANCODE::F14);
	mapScanCode(DIK_F15, AGN::SCANCODE::F15);
	//mapScanCode(DIK_NUMLOCK, AGN::SCANCODE::??); // TODO:
	mapScanCode(DIK_SCROLL, AGN::SCANCODE::SCROLLLOCK);
	mapScanCode(DIK_NUMPAD7, AGN::SCANCODE::KP_7);
	mapScanCode(DIK_NUMPAD8, AGN::SCANCODE::KP_8);
	mapScanCode(DIK_NUMPAD9, AGN::SCANCODE::KP_9);
	mapScanCode(DIK_SUBTRACT, AGN::SCANCODE::KP_MEMSUBTRACT);
	mapScanCode(DIK_NUMPAD4, AGN::SCANCODE::KP_4);
	mapScanCode(DIK_NUMPAD5, AGN::SCANCODE::KP_5);
	mapScanCode(DIK_NUMPAD6, AGN::SCANCODE::KP_6);
	mapScanCode(DIK_ADD, AGN::SCANCODE::KP_MEMADD);
	mapScanCode(DIK_NUMPAD1, AGN::SCANCODE::KP_1);
	mapScanCode(DIK_NUMPAD2, AGN::SCANCODE::KP_2);
	mapScanCode(DIK_NUMPAD3, AGN::SCANCODE::KP_3);
	mapScanCode(DIK_NUMPAD0, AGN::SCANCODE::KP_0);
	mapScanCode(DIK_DECIMAL, AGN::SCANCODE::KP_DECIMAL);
	//mapScanCode(DIK_OEM_102, AGN::SCANCODE::);
	//mapScanCode(DIK_KANA, AGN::SCANCODE::);
	//mapScanCode(DIK_ABNT_C1, AGN::SCANCODE::);
	//mapScanCode(DIK_CONVERT, AGN::SCANCODE::);
	//mapScanCode(DIK_NOCONVERT, AGN::SCANCODE::);
	//mapScanCode(DIK_YEN, AGN::SCANCODE::);
	//mapScanCode(DIK_ABNT_C2, AGN::SCANCODE::);
	mapScanCode(DIK_NUMPADEQUALS, AGN::SCANCODE::KP_EQUALS);
	mapScanCode(DIK_PREVTRACK, AGN::SCANCODE::AUDIOPREV);
	//mapScanCode(DIK_AT, AGN::SCANCODE::);
	mapScanCode(DIK_COLON, AGN::SCANCODE::KP_COLON);
	//mapScanCode(DIK_UNDERLINE, AGN::SCANCODE::)
	//mapScanCode(DIK_KANJI, AGN::SCANCODE::);
	mapScanCode(DIK_STOP, AGN::SCANCODE::STOP);
	//mapScanCode(DIK_AX, AGN::SCANCODE::);
	//mapScanCode(DIK_UNLABELED, AGN::SCANCODE::);
	mapScanCode(DIK_NEXTTRACK, AGN::SCANCODE::AUDIONEXT);
	mapScanCode(DIK_NUMPADENTER, AGN::SCANCODE::KP_ENTER);
	mapScanCode(DIK_RCONTROL, AGN::SCANCODE::RCTRL);
	mapScanCode(DIK_MUTE, AGN::SCANCODE::MUTE);
	mapScanCode(DIK_CALCULATOR, AGN::SCANCODE::CALCULATOR);
	mapScanCode(DIK_PLAYPAUSE, AGN::SCANCODE::AUDIOPLAY);
	mapScanCode(DIK_MEDIASTOP, AGN::SCANCODE::AC_STOP);
	mapScanCode(DIK_VOLUMEDOWN, AGN::SCANCODE::VOLUMEDOWN);
	mapScanCode(DIK_VOLUMEUP, AGN::SCANCODE::VOLUMEUP);
	mapScanCode(DIK_WEBHOME, AGN::SCANCODE::AC_HOME);
	mapScanCode(DIK_NUMPADCOMMA, AGN::SCANCODE::KP_COMMA);
	mapScanCode(DIK_DIVIDE, AGN::SCANCODE::KP_DIVIDE);
	mapScanCode(DIK_SYSRQ, AGN::SCANCODE::SYSREQ);
	//mapScanCode(DIK_RMENU, AGN::SCANCODE::);
	mapScanCode(DIK_PAUSE, AGN::SCANCODE::PAUSE);
	mapScanCode(DIK_HOME, AGN::SCANCODE::HOME);
	mapScanCode(DIK_UP, AGN::SCANCODE::UP);
	mapScanCode(DIK_PRIOR, AGN::SCANCODE::PRIOR);
	mapScanCode(DIK_LEFT, AGN::SCANCODE::LEFT);
	mapScanCode(DIK_RIGHT, AGN::SCANCODE::RIGHT);
	mapScanCode(DIK_END, AGN::SCANCODE::END);
	mapScanCode(DIK_DOWN, AGN::SCANCODE::DOWN);
	mapScanCode(DIK_NEXT, AGN::SCANCODE::AUDIONEXT);
	mapScanCode(DIK_INSERT, AGN::SCANCODE::INSERT);
	mapScanCode(DIK_DELETE, AGN::SCANCODE::ADELETE);
	//mapScanCode(DIK_LWIN, AGN::SCANCODE::); // TODO:
	//mapScanCode(DIK_RWIN, AGN::SCANCODE::); // TODO:
	mapScanCode(DIK_APPS, AGN::SCANCODE::APP1);
	mapScanCode(DIK_POWER, AGN::SCANCODE::POWER);
	mapScanCode(DIK_SLEEP, AGN::SCANCODE::SLEEP);
	//mapScanCode(DIK_WAKE, AGN::SCANCODE::);
	mapScanCode(DIK_WEBSEARCH, AGN::SCANCODE::AC_SEARCH);
	mapScanCode(DIK_WEBFAVORITES, AGN::SCANCODE::AC_BOOKMARKS);
	mapScanCode(DIK_WEBREFRESH, AGN::SCANCODE::AC_REFRESH);
	mapScanCode(DIK_WEBSTOP, AGN::SCANCODE::AC_STOP);
	mapScanCode(DIK_WEBFORWARD, AGN::SCANCODE::AC_FORWARD);
	mapScanCode(DIK_WEBBACK, AGN::SCANCODE::AC_BACK);
	mapScanCode(DIK_MYCOMPUTER, AGN::SCANCODE::COMPUTER);
	mapScanCode(DIK_MAIL, AGN::SCANCODE::MAIL);
	mapScanCode(DIK_MEDIASELECT, AGN::SCANCODE::MEDIASELECT);
}

AGN::SCANCODE AGN::AInputDX11::getAGNScanCode(uint32_t a_dx11scancode)
{
	return (AGN::SCANCODE)s_dx11mappedAGNScancodes[a_dx11scancode];
}

void AGN::AInputDX11::registerMouseClick(AGN::AGN_MOUSECODE a_mouseButton)
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

void AGN::AInputDX11::registerMouseHold(AGN::AGN_MOUSECODE a_mouseButton, bool a_held)
{
	if (m_mouse[(int)a_mouseButton] != a_held)
	{
		m_click[(int)a_mouseButton] = true;
	}
	m_mouse[(int)a_mouseButton] = a_held;
}

void AGN::AInputDX11::registerHold(AGN::SCANCODE a_key, bool a_held)
{
	if (m_held[(int)a_key] != a_held)
	{
		m_held[(int)a_key] = a_held;
		m_heldChange[(int)a_key] = true;
	}

	// TODO: if down, add to input string of characters (for inputfields and such)
	//if (m_doTrackInput && a_held) onInputCharacter(a_key);
}

void AGN::AInputDX11::registerFrameCompletion()
{
	// set all heldchange back to false
	memset(m_heldChange, false, sizeof(m_heldChange));
	m_scrollAmount = 0;
}

void AGN::AInputDX11::registerMouseMotion(int a_mouseX, int a_mouseY)
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

bool AGN::AInputDX11::getKeyDown(AGN::SCANCODE a_key)
{
	const bool retVal = m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::AInputDX11::getKeyUp(AGN::SCANCODE a_key)
{
	const bool retVal = !m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::AInputDX11::getKey(AGN::SCANCODE a_key)
{
	return m_held[(int)a_key];
}

bool AGN::AInputDX11::getMouseDown(AGN::AGN_MOUSECODE a_mouseButton)
{
	if (m_click[(int)a_mouseButton] && m_mouse[(int)a_mouseButton])
	{
		m_click[(int)a_mouseButton] = false;
		return true;
	}
	else return false;
}

bool AGN::AInputDX11::getMouseUp(AGN::AGN_MOUSECODE a_mouseButton)
{
	if (m_click[(int)a_mouseButton] && !m_mouse[(int)a_mouseButton])
	{
		m_click[(int)a_mouseButton] = false;
		return true;
	}
	else return false;
}

bool AGN::AInputDX11::getMouse(AGN::AGN_MOUSECODE a_mouseButton)
{
	//uint32_t mouseState = SDL_GetMouseState(NULL, NULL)&SDL_BUTTON((uint8_t)a_mouseButton);
	//uint32_t buttonState = SDL_BUTTON((uint32_t)a_mouseButton);
	//return (mouseState == buttonState);
	return false; // TODO: Restore
}

bool AGN::AInputDX11::getCapslock()
{
	//return bool((SDL_GetModState() & KMOD_CAPS) == 1);
	return false; // TODO: Restore
}

void AGN::AInputDX11::addMouseMotionHandler(AGN::IAInputMouseMotionHandler* a_motionHandler)
{
	m_mouseMotionHandlers.push_back(a_motionHandler);
}

void AGN::AInputDX11::removeMouseMotionHandler(AGN::IAInputMouseMotionHandler* a_motionHandler)
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

void AGN::AInputDX11::addMouseClickHandler(class IAInputMouseClickHandler* a_clickHandler)
{
	m_mouseClickHandlers.push_back(a_clickHandler);
}

void AGN::AInputDX11::removeMouseClickHandler(class IAInputMouseClickHandler* a_clickHandler)
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
