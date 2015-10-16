#include "asharedh.hpp"
#include "ainput_dx11.hpp"
#include "dinput.h"

// set static reference of g_input to the DX11 input
AGN::AInputDX11 inputDX11 = AGN::AInputDX11();
AGN::IAInput& g_input = inputDX11;

static uint16_t s_dx11mappedAGNScancodes[static_cast<unsigned int>(AGN::AGN_SCANCODE::NUM_SCANCODES)];

static void mapScanCode(uint16_t a_dx11scancode, AGN::AGN_SCANCODE a_agnCode)
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
	memset(s_dx11mappedAGNScancodes, 0, static_cast<unsigned int>(AGN::AGN_SCANCODE::NUM_SCANCODES) * sizeof(uint16_t));
	mapScanCode(DIK_1, AGN_SCANCODE::TOPNUM_1);
	mapScanCode(DIK_2, AGN_SCANCODE::TOPNUM_2);
	mapScanCode(DIK_3, AGN_SCANCODE::TOPNUM_3);
	mapScanCode(DIK_4, AGN_SCANCODE::TOPNUM_4);
	mapScanCode(DIK_5, AGN_SCANCODE::TOPNUM_5);
	mapScanCode(DIK_6, AGN_SCANCODE::TOPNUM_6);
	mapScanCode(DIK_7, AGN_SCANCODE::TOPNUM_7);
	mapScanCode(DIK_8, AGN_SCANCODE::TOPNUM_8);
	mapScanCode(DIK_9, AGN_SCANCODE::TOPNUM_9);
	mapScanCode(DIK_0, AGN_SCANCODE::TOPNUM_0);
	mapScanCode(DIK_MINUS, AGN_SCANCODE::MINUS);
	mapScanCode(DIK_EQUALS, AGN_SCANCODE::EQUALS);
	mapScanCode(DIK_BACK, AGN_SCANCODE::BACKSPACE);
	mapScanCode(DIK_TAB, AGN_SCANCODE::TAB);
	mapScanCode(DIK_Q, AGN_SCANCODE::Q);
	mapScanCode(DIK_W, AGN_SCANCODE::W);
	mapScanCode(DIK_E, AGN_SCANCODE::E);
	mapScanCode(DIK_R, AGN_SCANCODE::R);
	mapScanCode(DIK_T, AGN_SCANCODE::T);
	mapScanCode(DIK_Y, AGN_SCANCODE::Y);
	mapScanCode(DIK_U, AGN_SCANCODE::U);
	mapScanCode(DIK_I, AGN_SCANCODE::I);
	mapScanCode(DIK_O, AGN_SCANCODE::O);
	mapScanCode(DIK_P, AGN_SCANCODE::P);
	mapScanCode(DIK_LBRACKET, AGN_SCANCODE::LEFTBRACKET);
	mapScanCode(DIK_RBRACKET, AGN_SCANCODE::RIGHTBRACKET);
	mapScanCode(DIK_RETURN, AGN_SCANCODE::RETURN);
	mapScanCode(DIK_LCONTROL, AGN_SCANCODE::LCTRL);
	mapScanCode(DIK_A, AGN_SCANCODE::A);
	mapScanCode(DIK_S, AGN_SCANCODE::S);
	mapScanCode(DIK_D, AGN_SCANCODE::D);
	mapScanCode(DIK_F, AGN_SCANCODE::F);
	mapScanCode(DIK_G, AGN_SCANCODE::G);
	mapScanCode(DIK_H, AGN_SCANCODE::H);
	mapScanCode(DIK_J, AGN_SCANCODE::J);
	mapScanCode(DIK_K, AGN_SCANCODE::K);
	mapScanCode(DIK_L, AGN_SCANCODE::L);
	mapScanCode(DIK_SEMICOLON, AGN_SCANCODE::SEMICOLON);
	mapScanCode(DIK_APOSTROPHE, AGN_SCANCODE::APOSTROPHE);
	mapScanCode(DIK_GRAVE, AGN_SCANCODE::GRAVE);
	mapScanCode(DIK_LSHIFT, AGN_SCANCODE::LSHIFT);
	mapScanCode(DIK_BACKSLASH, AGN_SCANCODE::BACKSLASH);
	mapScanCode(DIK_Z, AGN_SCANCODE::Z);
	mapScanCode(DIK_X, AGN_SCANCODE::X);
	mapScanCode(DIK_C, AGN_SCANCODE::C);
	mapScanCode(DIK_V, AGN_SCANCODE::V);
	mapScanCode(DIK_B, AGN_SCANCODE::B);
	mapScanCode(DIK_N, AGN_SCANCODE::N);
	mapScanCode(DIK_M, AGN_SCANCODE::M);
	mapScanCode(DIK_COMMA, AGN_SCANCODE::COMMA);
	mapScanCode(DIK_PERIOD, AGN_SCANCODE::PERIOD);
	mapScanCode(DIK_SLASH, AGN_SCANCODE::SLASH);
	mapScanCode(DIK_RSHIFT, AGN_SCANCODE::RSHIFT);
	mapScanCode(DIK_MULTIPLY, AGN_SCANCODE::KP_MULTIPLY);
	//mapScanCode(DIK_LMENU, AGN_SCANCODE::??);	// TODO
	mapScanCode(DIK_SPACE, AGN_SCANCODE::SPACE);
	//mapScanCode(DIK_CAPITAL, AGN_SCANCODE::??); // TODO
	mapScanCode(DIK_F1, AGN_SCANCODE::F1);
	mapScanCode(DIK_F2, AGN_SCANCODE::F2);
	mapScanCode(DIK_F3, AGN_SCANCODE::F3);
	mapScanCode(DIK_F4, AGN_SCANCODE::F4);
	mapScanCode(DIK_F5, AGN_SCANCODE::F5);
	mapScanCode(DIK_F6, AGN_SCANCODE::F6);
	mapScanCode(DIK_F7, AGN_SCANCODE::F7);
	mapScanCode(DIK_F8, AGN_SCANCODE::F8);
	mapScanCode(DIK_F9, AGN_SCANCODE::F9);
	mapScanCode(DIK_F10, AGN_SCANCODE::F10);
	mapScanCode(DIK_F11, AGN_SCANCODE::F11);
	mapScanCode(DIK_F12, AGN_SCANCODE::F12);
	mapScanCode(DIK_F13, AGN_SCANCODE::F13);
	mapScanCode(DIK_F14, AGN_SCANCODE::F14);
	mapScanCode(DIK_F15, AGN_SCANCODE::F15);
	//mapScanCode(DIK_NUMLOCK, AGN_SCANCODE::??); // TODO:
	mapScanCode(DIK_SCROLL, AGN_SCANCODE::SCROLLLOCK);
	mapScanCode(DIK_NUMPAD7, AGN_SCANCODE::KP_7);
	mapScanCode(DIK_NUMPAD8, AGN_SCANCODE::KP_8);
	mapScanCode(DIK_NUMPAD9, AGN_SCANCODE::KP_9);
	mapScanCode(DIK_SUBTRACT, AGN_SCANCODE::KP_MEMSUBTRACT);
	mapScanCode(DIK_NUMPAD4, AGN_SCANCODE::KP_4);
	mapScanCode(DIK_NUMPAD5, AGN_SCANCODE::KP_5);
	mapScanCode(DIK_NUMPAD6, AGN_SCANCODE::KP_6);
	mapScanCode(DIK_ADD, AGN_SCANCODE::KP_MEMADD);
	mapScanCode(DIK_NUMPAD1, AGN_SCANCODE::KP_1);
	mapScanCode(DIK_NUMPAD2, AGN_SCANCODE::KP_2);
	mapScanCode(DIK_NUMPAD3, AGN_SCANCODE::KP_3);
	mapScanCode(DIK_NUMPAD0, AGN_SCANCODE::KP_0);
	mapScanCode(DIK_DECIMAL, AGN_SCANCODE::KP_DECIMAL);
	//mapScanCode(DIK_OEM_102, AGN_SCANCODE::);
	//mapScanCode(DIK_KANA, AGN_SCANCODE::);
	//mapScanCode(DIK_ABNT_C1, AGN_SCANCODE::);
	//mapScanCode(DIK_CONVERT, AGN_SCANCODE::);
	//mapScanCode(DIK_NOCONVERT, AGN_SCANCODE::);
	//mapScanCode(DIK_YEN, AGN_SCANCODE::);
	//mapScanCode(DIK_ABNT_C2, AGN_SCANCODE::);
	mapScanCode(DIK_NUMPADEQUALS, AGN_SCANCODE::KP_EQUALS);
	mapScanCode(DIK_PREVTRACK, AGN_SCANCODE::AUDIOPREV);
	//mapScanCode(DIK_AT, AGN_SCANCODE::);
	mapScanCode(DIK_COLON, AGN_SCANCODE::KP_COLON);
	//mapScanCode(DIK_UNDERLINE, AGN_SCANCODE::)
	//mapScanCode(DIK_KANJI, AGN_SCANCODE::);
	mapScanCode(DIK_STOP, AGN_SCANCODE::STOP);
	//mapScanCode(DIK_AX, AGN_SCANCODE::);
	//mapScanCode(DIK_UNLABELED, AGN_SCANCODE::);
	mapScanCode(DIK_NEXTTRACK, AGN_SCANCODE::AUDIONEXT);
	mapScanCode(DIK_NUMPADENTER, AGN_SCANCODE::KP_ENTER);
	mapScanCode(DIK_RCONTROL, AGN_SCANCODE::RCTRL);
	mapScanCode(DIK_MUTE, AGN_SCANCODE::MUTE);
	mapScanCode(DIK_CALCULATOR, AGN_SCANCODE::CALCULATOR);
	mapScanCode(DIK_PLAYPAUSE, AGN_SCANCODE::AUDIOPLAY);
	mapScanCode(DIK_MEDIASTOP, AGN_SCANCODE::AC_STOP);
	mapScanCode(DIK_VOLUMEDOWN, AGN_SCANCODE::VOLUMEDOWN);
	mapScanCode(DIK_VOLUMEUP, AGN_SCANCODE::VOLUMEUP);
	mapScanCode(DIK_WEBHOME, AGN_SCANCODE::AC_HOME);
	mapScanCode(DIK_NUMPADCOMMA, AGN_SCANCODE::KP_COMMA);
	mapScanCode(DIK_DIVIDE, AGN_SCANCODE::KP_DIVIDE);
	mapScanCode(DIK_SYSRQ, AGN_SCANCODE::SYSREQ);
	//mapScanCode(DIK_RMENU, AGN_SCANCODE::);
	mapScanCode(DIK_PAUSE, AGN_SCANCODE::PAUSE);
	mapScanCode(DIK_HOME, AGN_SCANCODE::HOME);
	mapScanCode(DIK_UP, AGN_SCANCODE::UP);
	mapScanCode(DIK_PRIOR, AGN_SCANCODE::PRIOR);
	mapScanCode(DIK_LEFT, AGN_SCANCODE::LEFT);
	mapScanCode(DIK_RIGHT, AGN_SCANCODE::RIGHT);
	mapScanCode(DIK_END, AGN_SCANCODE::END);
	mapScanCode(DIK_DOWN, AGN_SCANCODE::DOWN);
	mapScanCode(DIK_NEXT, AGN_SCANCODE::AUDIONEXT);
	mapScanCode(DIK_INSERT, AGN_SCANCODE::INSERT);
	mapScanCode(DIK_DELETE, AGN_SCANCODE::ADELETE);
	//mapScanCode(DIK_LWIN, AGN_SCANCODE::); // TODO:
	//mapScanCode(DIK_RWIN, AGN_SCANCODE::); // TODO:
	mapScanCode(DIK_APPS, AGN_SCANCODE::APP1);
	mapScanCode(DIK_POWER, AGN_SCANCODE::POWER);
	mapScanCode(DIK_SLEEP, AGN_SCANCODE::SLEEP);
	//mapScanCode(DIK_WAKE, AGN_SCANCODE::);
	mapScanCode(DIK_WEBSEARCH, AGN_SCANCODE::AC_SEARCH);
	mapScanCode(DIK_WEBFAVORITES, AGN_SCANCODE::AC_BOOKMARKS);
	mapScanCode(DIK_WEBREFRESH, AGN_SCANCODE::AC_REFRESH);
	mapScanCode(DIK_WEBSTOP, AGN_SCANCODE::AC_STOP);
	mapScanCode(DIK_WEBFORWARD, AGN_SCANCODE::AC_FORWARD);
	mapScanCode(DIK_WEBBACK, AGN_SCANCODE::AC_BACK);
	mapScanCode(DIK_MYCOMPUTER, AGN_SCANCODE::COMPUTER);
	mapScanCode(DIK_MAIL, AGN_SCANCODE::MAIL);
	mapScanCode(DIK_MEDIASELECT, AGN_SCANCODE::MEDIASELECT);
}

AGN::AGN_SCANCODE AGN::AInputDX11::getAGNScanCode(uint32_t a_dx11scancode)
{
	return (AGN::AGN_SCANCODE)s_dx11mappedAGNScancodes[a_dx11scancode];
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

void AGN::AInputDX11::registerHold(AGN::AGN_SCANCODE a_key, bool a_held)
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

bool AGN::AInputDX11::getKeyDown(AGN::AGN_SCANCODE a_key)
{
	const bool retVal = m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::AInputDX11::getKeyUp(AGN::AGN_SCANCODE a_key)
{
	const bool retVal = !m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::AInputDX11::getKey(AGN::AGN_SCANCODE a_key)
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
