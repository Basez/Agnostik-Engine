#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "input_dx11.hpp"
#define DIRECTINPUT_VERSION 0x0800 // gets rid of dinput.h waring (gives a silly warning if input version is not specified, 0x0800 is default)
#include "dinput.h"

// set static reference of g_input to the DX11 input
AGN::InputDX11 inputDX11 = AGN::InputDX11();
AGN::IInput& g_input = inputDX11;

static uint16_t s_dx11mappedAGNScancodes[static_cast<unsigned int>(AGN::ESCANCODE::NUM_SCANCODES)];

static void mapScanCode(uint16_t a_dx11scancode, AGN::ESCANCODE a_agnCode)
{
	s_dx11mappedAGNScancodes[a_dx11scancode] = (uint8_t)a_agnCode;
}

AGN::InputDX11::InputDX11()
	: m_mouseX(0)
	, m_mouseY(0)
{
	// initialize everything to false
	memset(m_heldChange, false, sizeof(m_heldChange));
	memset(m_held, false, sizeof(m_held));
	memset(m_mouse, false, sizeof(m_mouse));
	memset(m_click, false, sizeof(m_click));

	// map keys
	memset(s_dx11mappedAGNScancodes, 0, static_cast<unsigned int>(AGN::ESCANCODE::NUM_SCANCODES) * sizeof(uint16_t));
	mapScanCode(DIK_1, AGN::ESCANCODE::TOPNUM_1);
	mapScanCode(DIK_2, AGN::ESCANCODE::TOPNUM_2);
	mapScanCode(DIK_3, AGN::ESCANCODE::TOPNUM_3);
	mapScanCode(DIK_4, AGN::ESCANCODE::TOPNUM_4);
	mapScanCode(DIK_5, AGN::ESCANCODE::TOPNUM_5);
	mapScanCode(DIK_6, AGN::ESCANCODE::TOPNUM_6);
	mapScanCode(DIK_7, AGN::ESCANCODE::TOPNUM_7);
	mapScanCode(DIK_8, AGN::ESCANCODE::TOPNUM_8);
	mapScanCode(DIK_9, AGN::ESCANCODE::TOPNUM_9);
	mapScanCode(DIK_0, AGN::ESCANCODE::TOPNUM_0);
	mapScanCode(DIK_MINUS, AGN::ESCANCODE::MINUS);
	mapScanCode(DIK_EQUALS, AGN::ESCANCODE::EQUALS);
	mapScanCode(DIK_BACK, AGN::ESCANCODE::BACKSPACE);
	mapScanCode(DIK_TAB, AGN::ESCANCODE::TAB);
	mapScanCode(DIK_Q, AGN::ESCANCODE::Q);
	mapScanCode(DIK_W, AGN::ESCANCODE::W);
	mapScanCode(DIK_E, AGN::ESCANCODE::E);
	mapScanCode(DIK_R, AGN::ESCANCODE::R);
	mapScanCode(DIK_T, AGN::ESCANCODE::T);
	mapScanCode(DIK_Y, AGN::ESCANCODE::Y);
	mapScanCode(DIK_U, AGN::ESCANCODE::U);
	mapScanCode(DIK_I, AGN::ESCANCODE::I);
	mapScanCode(DIK_O, AGN::ESCANCODE::O);
	mapScanCode(DIK_P, AGN::ESCANCODE::P);
	mapScanCode(DIK_LBRACKET, AGN::ESCANCODE::LEFTBRACKET);
	mapScanCode(DIK_RBRACKET, AGN::ESCANCODE::RIGHTBRACKET);
	mapScanCode(DIK_RETURN, AGN::ESCANCODE::RETURN);
	mapScanCode(DIK_LCONTROL, AGN::ESCANCODE::LCTRL);
	mapScanCode(DIK_A, AGN::ESCANCODE::A);
	mapScanCode(DIK_S, AGN::ESCANCODE::S);
	mapScanCode(DIK_D, AGN::ESCANCODE::D);
	mapScanCode(DIK_F, AGN::ESCANCODE::F);
	mapScanCode(DIK_G, AGN::ESCANCODE::G);
	mapScanCode(DIK_H, AGN::ESCANCODE::H);
	mapScanCode(DIK_J, AGN::ESCANCODE::J);
	mapScanCode(DIK_K, AGN::ESCANCODE::K);
	mapScanCode(DIK_L, AGN::ESCANCODE::L);
	mapScanCode(DIK_SEMICOLON, AGN::ESCANCODE::SEMICOLON);
	mapScanCode(DIK_APOSTROPHE, AGN::ESCANCODE::APOSTROPHE);
	mapScanCode(DIK_GRAVE, AGN::ESCANCODE::GRAVE);
	mapScanCode(DIK_LSHIFT, AGN::ESCANCODE::LSHIFT);
	mapScanCode(DIK_BACKSLASH, AGN::ESCANCODE::BACKSLASH);
	mapScanCode(DIK_Z, AGN::ESCANCODE::Z);
	mapScanCode(DIK_X, AGN::ESCANCODE::X);
	mapScanCode(DIK_C, AGN::ESCANCODE::C);
	mapScanCode(DIK_V, AGN::ESCANCODE::V);
	mapScanCode(DIK_B, AGN::ESCANCODE::B);
	mapScanCode(DIK_N, AGN::ESCANCODE::N);
	mapScanCode(DIK_M, AGN::ESCANCODE::M);
	mapScanCode(DIK_COMMA, AGN::ESCANCODE::COMMA);
	mapScanCode(DIK_PERIOD, AGN::ESCANCODE::PERIOD);
	mapScanCode(DIK_SLASH, AGN::ESCANCODE::SLASH);
	mapScanCode(DIK_RSHIFT, AGN::ESCANCODE::RSHIFT);
	mapScanCode(DIK_MULTIPLY, AGN::ESCANCODE::KP_MULTIPLY);
	//mapScanCode(DIK_LMENU, AGN::ESCANCODE::??);	// TODO
	mapScanCode(DIK_SPACE, AGN::ESCANCODE::SPACE);
	//mapScanCode(DIK_CAPITAL, AGN::ESCANCODE::??); // TODO
	mapScanCode(DIK_F1, AGN::ESCANCODE::F1);
	mapScanCode(DIK_F2, AGN::ESCANCODE::F2);
	mapScanCode(DIK_F3, AGN::ESCANCODE::F3);
	mapScanCode(DIK_F4, AGN::ESCANCODE::F4);
	mapScanCode(DIK_F5, AGN::ESCANCODE::F5);
	mapScanCode(DIK_F6, AGN::ESCANCODE::F6);
	mapScanCode(DIK_F7, AGN::ESCANCODE::F7);
	mapScanCode(DIK_F8, AGN::ESCANCODE::F8);
	mapScanCode(DIK_F9, AGN::ESCANCODE::F9);
	mapScanCode(DIK_F10, AGN::ESCANCODE::F10);
	mapScanCode(DIK_F11, AGN::ESCANCODE::F11);
	mapScanCode(DIK_F12, AGN::ESCANCODE::F12);
	mapScanCode(DIK_F13, AGN::ESCANCODE::F13);
	mapScanCode(DIK_F14, AGN::ESCANCODE::F14);
	mapScanCode(DIK_F15, AGN::ESCANCODE::F15);
	//mapScanCode(DIK_NUMLOCK, AGN::ESCANCODE::??); // TODO:
	mapScanCode(DIK_SCROLL, AGN::ESCANCODE::SCROLLLOCK);
	mapScanCode(DIK_NUMPAD7, AGN::ESCANCODE::KP_7);
	mapScanCode(DIK_NUMPAD8, AGN::ESCANCODE::KP_8);
	mapScanCode(DIK_NUMPAD9, AGN::ESCANCODE::KP_9);
	mapScanCode(DIK_SUBTRACT, AGN::ESCANCODE::KP_MEMSUBTRACT);
	mapScanCode(DIK_NUMPAD4, AGN::ESCANCODE::KP_4);
	mapScanCode(DIK_NUMPAD5, AGN::ESCANCODE::KP_5);
	mapScanCode(DIK_NUMPAD6, AGN::ESCANCODE::KP_6);
	mapScanCode(DIK_ADD, AGN::ESCANCODE::KP_MEMADD);
	mapScanCode(DIK_NUMPAD1, AGN::ESCANCODE::KP_1);
	mapScanCode(DIK_NUMPAD2, AGN::ESCANCODE::KP_2);
	mapScanCode(DIK_NUMPAD3, AGN::ESCANCODE::KP_3);
	mapScanCode(DIK_NUMPAD0, AGN::ESCANCODE::KP_0);
	mapScanCode(DIK_DECIMAL, AGN::ESCANCODE::KP_DECIMAL);
	//mapScanCode(DIK_OEM_102, AGN::ESCANCODE::);
	//mapScanCode(DIK_KANA, AGN::ESCANCODE::);
	//mapScanCode(DIK_ABNT_C1, AGN::ESCANCODE::);
	//mapScanCode(DIK_CONVERT, AGN::ESCANCODE::);
	//mapScanCode(DIK_NOCONVERT, AGN::ESCANCODE::);
	//mapScanCode(DIK_YEN, AGN::ESCANCODE::);
	//mapScanCode(DIK_ABNT_C2, AGN::ESCANCODE::);
	mapScanCode(DIK_NUMPADEQUALS, AGN::ESCANCODE::KP_EQUALS);
	mapScanCode(DIK_PREVTRACK, AGN::ESCANCODE::AUDIOPREV);
	//mapScanCode(DIK_AT, AGN::ESCANCODE::);
	mapScanCode(DIK_COLON, AGN::ESCANCODE::KP_COLON);
	//mapScanCode(DIK_UNDERLINE, AGN::ESCANCODE::)
	//mapScanCode(DIK_KANJI, AGN::ESCANCODE::);
	mapScanCode(DIK_STOP, AGN::ESCANCODE::STOP);
	//mapScanCode(DIK_AX, AGN::ESCANCODE::);
	//mapScanCode(DIK_UNLABELED, AGN::ESCANCODE::);
	mapScanCode(DIK_NEXTTRACK, AGN::ESCANCODE::AUDIONEXT);
	mapScanCode(DIK_NUMPADENTER, AGN::ESCANCODE::KP_ENTER);
	mapScanCode(DIK_RCONTROL, AGN::ESCANCODE::RCTRL);
	mapScanCode(DIK_MUTE, AGN::ESCANCODE::MUTE);
	mapScanCode(DIK_CALCULATOR, AGN::ESCANCODE::CALCULATOR);
	mapScanCode(DIK_PLAYPAUSE, AGN::ESCANCODE::AUDIOPLAY);
	mapScanCode(DIK_MEDIASTOP, AGN::ESCANCODE::AC_STOP);
	mapScanCode(DIK_VOLUMEDOWN, AGN::ESCANCODE::VOLUMEDOWN);
	mapScanCode(DIK_VOLUMEUP, AGN::ESCANCODE::VOLUMEUP);
	mapScanCode(DIK_WEBHOME, AGN::ESCANCODE::AC_HOME);
	mapScanCode(DIK_NUMPADCOMMA, AGN::ESCANCODE::KP_COMMA);
	mapScanCode(DIK_DIVIDE, AGN::ESCANCODE::KP_DIVIDE);
	mapScanCode(DIK_SYSRQ, AGN::ESCANCODE::SYSREQ);
	//mapScanCode(DIK_RMENU, AGN::ESCANCODE::);
	mapScanCode(DIK_PAUSE, AGN::ESCANCODE::PAUSE);
	mapScanCode(DIK_HOME, AGN::ESCANCODE::HOME);
	mapScanCode(DIK_UP, AGN::ESCANCODE::UP);
	mapScanCode(DIK_PRIOR, AGN::ESCANCODE::PRIOR);
	mapScanCode(DIK_LEFT, AGN::ESCANCODE::LEFT);
	mapScanCode(DIK_RIGHT, AGN::ESCANCODE::RIGHT);
	mapScanCode(DIK_END, AGN::ESCANCODE::END);
	mapScanCode(DIK_DOWN, AGN::ESCANCODE::DOWN);
	mapScanCode(DIK_NEXT, AGN::ESCANCODE::AUDIONEXT);
	mapScanCode(DIK_INSERT, AGN::ESCANCODE::INSERT);
	mapScanCode(DIK_DELETE, AGN::ESCANCODE::ADELETE);
	//mapScanCode(DIK_LWIN, AGN::ESCANCODE::); // TODO:
	//mapScanCode(DIK_RWIN, AGN::ESCANCODE::); // TODO:
	mapScanCode(DIK_APPS, AGN::ESCANCODE::APP1);
	mapScanCode(DIK_POWER, AGN::ESCANCODE::POWER);
	mapScanCode(DIK_SLEEP, AGN::ESCANCODE::SLEEP);
	//mapScanCode(DIK_WAKE, AGN::ESCANCODE::);
	mapScanCode(DIK_WEBSEARCH, AGN::ESCANCODE::AC_SEARCH);
	mapScanCode(DIK_WEBFAVORITES, AGN::ESCANCODE::AC_BOOKMARKS);
	mapScanCode(DIK_WEBREFRESH, AGN::ESCANCODE::AC_REFRESH);
	mapScanCode(DIK_WEBSTOP, AGN::ESCANCODE::AC_STOP);
	mapScanCode(DIK_WEBFORWARD, AGN::ESCANCODE::AC_FORWARD);
	mapScanCode(DIK_WEBBACK, AGN::ESCANCODE::AC_BACK);
	mapScanCode(DIK_MYCOMPUTER, AGN::ESCANCODE::COMPUTER);
	mapScanCode(DIK_MAIL, AGN::ESCANCODE::MAIL);
	mapScanCode(DIK_MEDIASELECT, AGN::ESCANCODE::MEDIASELECT);
}

AGN::InputDX11::~InputDX11()
{
	// TODO: refactor into cleanup() function as its a static class and we never know exactly when it gets deleted
	//g_log.warning("TODO: CLEAN InputDX11::~InputDX11()");
}

AGN::ESCANCODE AGN::InputDX11::getAGNScanCode(uint32_t a_dx11scancode)
{
	return (AGN::ESCANCODE)s_dx11mappedAGNScancodes[a_dx11scancode];
}

void AGN::InputDX11::registerMouseClick(AGN::EMOUSECODE a_mouseButton)
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

void AGN::InputDX11::registerMouseHold(AGN::EMOUSECODE a_mouseButton, bool a_held)
{
	if (m_mouse[(int)a_mouseButton] != a_held)
	{
		m_click[(int)a_mouseButton] = true;
	}
	m_mouse[(int)a_mouseButton] = a_held;
}

void AGN::InputDX11::registerHold(AGN::ESCANCODE a_key, bool a_held)
{
	if (m_held[(int)a_key] != a_held)
	{
		m_held[(int)a_key] = a_held;
		m_heldChange[(int)a_key] = true;
	}

	// TODO: if down, add to input string of characters (for inputfields and such)
	//if (m_doTrackInput && a_held) onInputCharacter(a_key);
}

void AGN::InputDX11::registerFrameCompletion()
{
	// set all heldchange back to false
	memset(m_heldChange, false, sizeof(m_heldChange));
	m_scrollAmount = 0;
}

void AGN::InputDX11::registerMouseMotion(int a_mouseX, int a_mouseY)
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

bool AGN::InputDX11::getKeyDown(AGN::ESCANCODE a_key)
{
	const bool retVal = m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::InputDX11::getKeyUp(AGN::ESCANCODE a_key)
{
	const bool retVal = !m_held[(int)a_key] && m_heldChange[(int)a_key];
	return retVal;
}

bool AGN::InputDX11::getKey(AGN::ESCANCODE a_key)
{
	return m_held[(int)a_key];
}

bool AGN::InputDX11::getMouseDown(AGN::EMOUSECODE a_mouseButton)
{
	if (m_click[(int)a_mouseButton] && m_mouse[(int)a_mouseButton])
	{
		m_click[(int)a_mouseButton] = false;
		return true;
	}
	else return false;
}

bool AGN::InputDX11::getMouseUp(AGN::EMOUSECODE a_mouseButton)
{
	if (m_click[(int)a_mouseButton] && !m_mouse[(int)a_mouseButton])
	{
		m_click[(int)a_mouseButton] = false;
		return true;
	}
	else return false;
}

bool AGN::InputDX11::getMouse(AGN::EMOUSECODE a_mouseButton)
{
	return m_mouse[(int)a_mouseButton];
}

bool AGN::InputDX11::getCapslock()
{
	//return bool((SDL_GetModState() & KMOD_CAPS) == 1);
	return false; // TODO: Restore
}

void AGN::InputDX11::addMouseMotionHandler(AGN::IInputMouseMotionHandler* a_motionHandler)
{
	m_mouseMotionHandlers.push_back(a_motionHandler);
}

void AGN::InputDX11::removeMouseMotionHandler(AGN::IInputMouseMotionHandler* a_motionHandler)
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

void AGN::InputDX11::addMouseClickHandler(class IInputMouseClickHandler* a_clickHandler)
{
	m_mouseClickHandlers.push_back(a_clickHandler);
}

void AGN::InputDX11::removeMouseClickHandler(class IInputMouseClickHandler* a_clickHandler)
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
