#pragma once

namespace AGN
{
	enum class AGN_MOUSECODE;
	enum class AGN_SCANCODE;

	class IAInput
	{
	public:
		virtual ~IAInput() {};

		// submitted to track input
		virtual void registerMouseClick(AGN::AGN_MOUSECODE a_mouseButton) = 0;
		virtual void registerMouseHold(AGN::AGN_MOUSECODE a_mouseButton, bool a_held) = 0;
		virtual void registerHold(AGN::AGN_SCANCODE a_key, bool a_held) = 0;
		virtual void registerFrameCompletion() = 0;
		virtual void registerMouseScroll(int a_scrollAmount) = 0;
		virtual void registerMouseMotion(int a_mouseX, int a_mouseY) = 0;

		// poll input functions
		virtual bool getKeyDown(AGN::AGN_SCANCODE a_key) = 0;
		virtual bool getKeyUp(AGN::AGN_SCANCODE a_key) = 0;
		virtual bool getKey(AGN::AGN_SCANCODE a_key) = 0;
		virtual bool getMouseDown(AGN::AGN_MOUSECODE a_mouseButton) = 0;
		virtual bool getMouseUp(AGN::AGN_MOUSECODE a_mouseButton) = 0;
		virtual bool getMouse(AGN::AGN_MOUSECODE a_mouseButton) = 0;
		virtual int getMouseX() = 0;
		virtual int getMouseY() = 0;
		virtual int getMouseXMove() = 0;
		virtual int getMouseYMove() = 0;
		virtual bool getCapslock() = 0;
		virtual const int getMouseScroll() = 0;

		// callback systems
		virtual void addMouseMotionHandler(class IAInputMouseMotionHandler* a_motionHandler) = 0;
		virtual void removeMouseMotionHandler(class IAInputMouseMotionHandler* a_motionHandler) = 0;
		virtual void addMouseClickHandler(class IAInputMouseClickHandler* a_clickHandler) = 0;
		virtual void removeMouseClickHandler(class IAInputMouseClickHandler* a_clickHandler) = 0;

		//virtual void setTrackInputIntoString(bool a_value, const int a_maxCharacters = -1) { m_doTrackInput = a_value; m_maxInputCharacters = a_maxCharacters; }			// TODO: inputString (for text fields)
		//virtual void setInputString(std::string a_string) { m_inputString = a_string; }		// TODO: inputString (for text fields)
		//virtual std::string getInputString() const { return m_inputString; }
		//virtual const bool getDoTrackInput() const { return m_doTrackInput; }
	};

	// interface for handling mouse motion events
	class IAInputMouseMotionHandler
	{
	public:
		virtual ~IAInputMouseMotionHandler() {};
		virtual void onMouseMotion(int a_mouseX, int a_mouseY) = 0;
	};

	// interface for handling mouse click events
	class IAInputMouseClickHandler
	{
	public:
		virtual ~IAInputMouseClickHandler() {};
		virtual void onMouseClick(int a_mouseX, int a_mouseY) = 0;
	};

	enum class AGN_MOUSECODE
	{
		// TODO: confirm these are set to SDL standards
		LEFT = 1,
		MIDDLE = 2,
		RIGHT = 3,

		NUM_MOUSECODES = 12
	};

	// Set to match SDL scancodes
	enum class AGN_SCANCODE
	{
		UNKNOWN = 0,
		A = 4,
		B = 5,
		C = 6,
		D = 7,
		E = 8,
		F = 9,
		G = 10,
		H = 11,
		I = 12,
		J = 13,
		K = 14,
		L = 15,
		M = 16,
		N = 17,
		O = 18,
		P = 19,
		Q = 20,
		R = 21,
		S = 22,
		T = 23,
		U = 24,
		V = 25,
		W = 26,
		X = 27,
		Y = 28,
		Z = 29,

		TOPNUM_1 = 30,
		TOPNUM_2 = 31,
		TOPNUM_3 = 32,
		TOPNUM_4 = 33,
		TOPNUM_5 = 34,
		TOPNUM_6 = 35,
		TOPNUM_7 = 36,
		TOPNUM_8 = 37,
		TOPNUM_9 = 38,
		TOPNUM_0 = 39,

		RETURN = 40,
		ESCAPE = 41,
		BACKSPACE = 42,
		TAB = 43,
		SPACE = 44,

		MINUS = 45,
		EQUALS = 46,
		LEFTBRACKET = 47,
		RIGHTBRACKET = 48,
		BACKSLASH = 49,
		NONUSHASH = 50,
		SEMICOLON = 51,
		APOSTROPHE = 52,
		GRAVE = 53,
		COMMA = 54,
		PERIOD = 55,
		SLASH = 56,

		CAPSLOCK = 57,

		F1 = 58,
		F2 = 59,
		F3 = 60,
		F4 = 61,
		F5 = 62,
		F6 = 63,
		F7 = 64,
		F8 = 65,
		F9 = 66,
		F10 = 67,
		F11 = 68,
		F12 = 69,

		PRINTSCREEN = 70,
		SCROLLLOCK = 71,
		PAUSE = 72,
		INSERT = 73,

		HOME = 74,
		PAGEUP = 75,
		ADELETE = 76,
		END = 77,
		PAGEDOWN = 78,
		RIGHT = 79,
		LEFT = 80,
		DOWN = 81,
		UP = 82,

		NUMLOCKCLEAR = 83,
		KP_DIVIDE = 84,
		KP_MULTIPLY = 85,
		KP_MINUS = 86,
		KP_PLUS = 87,
		KP_ENTER = 88,
		KP_1 = 89,
		KP_2 = 90,
		KP_3 = 91,
		KP_4 = 92,
		KP_5 = 93,
		KP_6 = 94,
		KP_7 = 95,
		KP_8 = 96,
		KP_9 = 97,
		KP_0 = 98,
		KP_PERIOD = 99,

		NONUSBACKSLASH = 100,
		APPLICATION = 101,
		POWER = 102,

		KP_EQUALS = 103,
		F13 = 104,
		F14 = 105,
		F15 = 106,
		F16 = 107,
		F17 = 108,
		F18 = 109,
		F19 = 110,
		F20 = 111,
		F21 = 112,
		F22 = 113,
		F23 = 114,
		F24 = 115,
		EXECUTE = 116,
		HELP = 117,
		MENU = 118,
		SELECT = 119,
		STOP = 120,
		AGAIN = 121,
		UNDO = 122,
		CUT = 123,
		COPY = 124,
		PASTE = 125,
		FIND = 126,
		MUTE = 127,
		VOLUMEUP = 128,
		VOLUMEDOWN = 129,

		KP_COMMA = 133,
		KP_EQUALSAS400 = 134,

		INTERNATIONAL1 = 135,
		INTERNATIONAL2 = 136,
		INTERNATIONAL3 = 137,
		INTERNATIONAL4 = 138,
		INTERNATIONAL5 = 139,
		INTERNATIONAL6 = 140,
		INTERNATIONAL7 = 141,
		INTERNATIONAL8 = 142,
		INTERNATIONAL9 = 143,
		LANG1 = 144,
		LANG2 = 145,
		LANG3 = 146,
		LANG4 = 147,
		LANG5 = 148,
		LANG6 = 149,
		LANG7 = 150,
		LANG8 = 151,
		LANG9 = 152,

		ALTERASE = 153,
		SYSREQ = 154,
		CANCEL = 155,
		CLEAR = 156,
		PRIOR = 157,
		RETURN2 = 158,
		SEPARATOR = 159,
		OOUT = 160,
		OPER = 161,
		CLEARAGAIN = 162,
		CRSEL = 163,
		EXSEL = 164,

		KP_00 = 176,
		KP_000 = 177,
		THOUSANDSSEPARATOR = 178,
		DECIMALSEPARATOR = 179,
		CURRENCYUNIT = 180,
		CURRENCYSUBUNIT = 181,
		KP_LEFTPAREN = 182,
		KP_RIGHTPAREN = 183,
		KP_LEFTBRACE = 184,
		KP_RIGHTBRACE = 185,
		KP_TAB = 186,
		KP_BACKSPACE = 187,
		KP_A = 188,
		KP_B = 189,
		KP_C = 190,
		KP_D = 191,
		KP_E = 192,
		KP_F = 193,
		KP_XOR = 194,
		KP_POWER = 195,
		KP_PERCENT = 196,
		KP_LESS = 197,
		KP_GREATER = 198,
		KP_AMPERSAND = 199,
		KP_DBLAMPERSAND = 200,
		KP_VERTICALBAR = 201,
		KP_DBLVERTICALBAR = 202,
		KP_COLON = 203,
		KP_HASH = 204,
		KP_SPACE = 205,
		KP_AT = 206,
		KP_EXCLAM = 207,
		KP_MEMSTORE = 208,
		KP_MEMRECALL = 209,
		KP_MEMCLEAR = 210,
		KP_MEMADD = 211,
		KP_MEMSUBTRACT = 212,
		KP_MEMMULTIPLY = 213,
		KP_MEMDIVIDE = 214,
		KP_PLUSMINUS = 215,
		KP_CLEAR = 216,
		KP_CLEARENTRY = 217,
		KP_BINARY = 218,
		KP_OCTAL = 219,
		KP_DECIMAL = 220,
		KP_HEXADECIMAL = 221,
		LCTRL = 224,
		LSHIFT = 225,
		LALT = 226,
		LGUI = 227,
		RCTRL = 228,
		RSHIFT = 229,
		RALT = 230,
		RGUI = 231,
		MODE = 257,
		AUDIONEXT = 258,
		AUDIOPREV = 259,
		AUDIOSTOP = 260,
		AUDIOPLAY = 261,
		AUDIOMUTE = 262,
		MEDIASELECT = 263,
		WWW = 264,
		MAIL = 265,
		CALCULATOR = 266,
		COMPUTER = 267,
		AC_SEARCH = 268,
		AC_HOME = 269,
		AC_BACK = 270,
		AC_FORWARD = 271,
		AC_STOP = 272,
		AC_REFRESH = 273,
		AC_BOOKMARKS = 274,

		BRIGHTNESSDOWN = 275,
		BRIGHTNESSUP = 276,
		DISPLAYSWITCH = 277,

		KBDILLUMTOGGLE = 278,
		KBDILLUMDOWN = 279,
		KBDILLUMUP = 280,
		EJECT = 281,
		SLEEP = 282,

		APP1 = 283,
		APP2 = 284,

		NUM_SCANCODES = 512
	};
}

// reference to the global logger (implemented in platform specific class)
extern AGN::IAInput& g_input;