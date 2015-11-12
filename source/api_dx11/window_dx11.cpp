#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "window_dx11.hpp"
#include "os_utils.hpp"
#include "i_input.hpp"
#include "input_dx11.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static const int MINIMUM_WINDOW_SIZE_X = 36;
static const int MINIMUM_WINDOW_SIZE_Y = 36;

// accessor for global window procedure callback function
static AGN::WindowDX11* g_window = nullptr;

// process messages sent to the window.
LRESULT CALLBACK AGN::windowProcedureCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: refactor?
	return g_window->onWindowEvent(hwnd, message, wParam, lParam);
}

AGN::WindowDX11::WindowDX11(glm::ivec2 a_dimentions)
	: m_dimentions(a_dimentions)
	, m_isDirty(false)
{
	g_window = this;

#ifdef AGN_DEBUG
	m_windowName = "Agnostik D3D11 - DEBUG";
	m_applicationName = "agnostik_dx11_debug";
#elif AGN_RELEASE
	m_windowName = std::string("Agnostik D3D11 - RELEASE");
	m_applicationName = "agnostik_dx11_release";
#else
	assert(false);
#endif

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &windowProcedureCallback;
	wndClass.hInstance = GetModuleHandle(nullptr);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = m_applicationName.c_str();

	if (!RegisterClassEx(&wndClass))
	{
		g_log.error("Could not register window class");
		return;
	}
	RECT windowRect = { 0, 0, a_dimentions.x, a_dimentions.y };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto moduleHandle = GetModuleHandle(nullptr);

	m_windowHandle = CreateWindowA(m_applicationName.c_str(), m_windowName.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, moduleHandle, nullptr);

	if (!m_windowHandle)
	{
		g_log.error("Failed to create Window");
		return;
	}

	ShowWindow(m_windowHandle, 10);
	UpdateWindow(m_windowHandle);
}

AGN::WindowDX11::~WindowDX11()
{
	DestroyWindow(m_windowHandle);

	UnregisterClass(m_applicationName.c_str(), GetModuleHandle(nullptr));
}

LRESULT CALLBACK AGN::WindowDX11::onWindowEvent(HWND a_hwnd, UINT a_message, WPARAM a_wParam, LPARAM a_lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (a_message)
	{
	case WM_PAINT:
	{
		// erase background
		hDC = BeginPaint(a_hwnd, &paintStruct);
		EndPaint(a_hwnd, &paintStruct);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_SIZE:
	{
		// hack to prevent screen resize event from propigating the first time the window is created
		static bool firstResize = true;
		if (firstResize)
		{
			firstResize = false;
			break;
		}

		RECT clientRect;
		GetClientRect(m_windowHandle, &clientRect);
		int newWidth = clientRect.right - clientRect.left;
		int newHeight = clientRect.bottom - clientRect.top;

		if (newWidth < MINIMUM_WINDOW_SIZE_X || newHeight < MINIMUM_WINDOW_SIZE_Y)
		{
			g_log.warning("too small window size: %ix%i", newWidth, newHeight);
			m_isDirty = false;
			break;
		}
		else
		{
			m_isDirty = true;
			break;
		}
		
	}

	case WM_KEYDOWN:
	{
		//MSG charMsg;
		// Get the unicode character (UTF-16)
		//unsigned int c = 0;
		// For printable characters, the next message will be WM_CHAR.
		// This message contains the character code we need to send the KeyPressed event. Inspired by the SDL 1.2 implementation.
		//if (PeekMessage(&charMsg, a_hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR)
		//{
		//	GetMessage(&charMsg, a_hwnd, 0, 0);
		//	c = charMsg.wParam;
		//}

		// TODO:
		bool shift = GetAsyncKeyState(VK_SHIFT) > 0;
		bool control = GetAsyncKeyState(VK_CONTROL) > 0;
		bool alt = GetAsyncKeyState(VK_MENU) > 0;

		uint32_t dx11ScanCode = (a_lParam & 0x00FF0000) >> 16;
		uint64_t dx11KeyCode = (uint64_t)a_wParam;
		auto agnScanKey = InputDX11::getAGNScanCode(dx11ScanCode);

		//g_log.debug("dx11ScanCode: %i, agnScanKey: %u", dx11ScanCode, (uint16_t)agnScanKey);

		g_input.registerHold(agnScanKey, true);


		//KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Pressed, shift, control, alt);
		//pWindow->OnKeyPressed(keyEventArgs);

		break;
	}
	case WM_KEYUP:
	{
		bool shift = GetAsyncKeyState(VK_SHIFT) > 0;
		bool control = GetAsyncKeyState(VK_CONTROL) > 0;
		bool alt = GetAsyncKeyState(VK_MENU) > 0;
		uint32_t dx11ScanCode = (a_lParam & 0x00FF0000) >> 16;
		uint64_t dx11KeyCode = (uint64_t)a_wParam;
		auto agnScanKey = InputDX11::getAGNScanCode(dx11ScanCode);

		g_input.registerHold(agnScanKey, false);

		// Determine which key was released by converting the key code and the scan code
		// to a printable character (if possible). Inspired by the SDL 1.2 implementation.
		//unsigned char keyboardState[256];
		//GetKeyboardState(keyboardState);
		//wchar_t translatedCharacters[4];
		//if (int result = ToUnicodeEx(a_wParam, scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
		//{
		//	c = translatedCharacters[0];
		//}

		//KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Released, shift, control, alt);
		//pWindow->OnKeyReleased(keyEventArgs);
		break;
	}
	case WM_MOUSEMOVE:
	{
		bool lButton = (a_wParam & MK_LBUTTON) != 0;
		bool rButton = (a_wParam & MK_RBUTTON) != 0;
		bool mButton = (a_wParam & MK_MBUTTON) != 0;
		bool shift = (a_wParam & MK_SHIFT) != 0;
		bool control = (a_wParam & MK_CONTROL) != 0;

		int mouseX = ((int)(short)LOWORD(a_lParam));
		int mouseY = ((int)(short)HIWORD(a_lParam));

		g_input.registerMouseMotion(mouseX, mouseY);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		g_input.registerMouseHold(AGN::EMOUSECODE::LEFT, true);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		g_input.registerMouseHold(AGN::EMOUSECODE::MIDDLE, true);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		g_input.registerMouseHold(AGN::EMOUSECODE::RIGHT, true);
		break;
	}
	case WM_LBUTTONUP:
	{
		g_input.registerMouseHold(AGN::EMOUSECODE::LEFT, false);
		g_input.registerMouseClick(AGN::EMOUSECODE::LEFT);
		break;
	}
	case WM_MBUTTONUP:
	{
		g_input.registerMouseHold(AGN::EMOUSECODE::MIDDLE, false);
		g_input.registerMouseClick(AGN::EMOUSECODE::MIDDLE);
		break;
	}
	case WM_RBUTTONUP:
	{
		g_input.registerMouseHold(AGN::EMOUSECODE::RIGHT, false);
		g_input.registerMouseClick(AGN::EMOUSECODE::RIGHT);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		g_input.registerMouseScroll(int(((int)(short)HIWORD(a_wParam)) / (float)WHEEL_DELTA));
		break;
	}
	
	default:
	{
		return DefWindowProc(a_hwnd, a_message, a_wParam, a_lParam);
	}
	}
	return 0;
}

void AGN::WindowDX11::setTitle(const char* a_title)
{
	m_windowName = a_title;
	SetWindowText(m_windowHandle, m_windowName.c_str());
}

void AGN::WindowDX11::showMessageBox(const char* a_title, const char* a_message)
{
	MessageBox(nullptr, TEXT(a_message), TEXT(a_title), MB_OK);
}

void AGN::WindowDX11::warpCursor(glm::ivec2 a_screenPosition)
{
	POINT mouse;
	mouse.x = a_screenPosition.x;
	mouse.y = a_screenPosition.y;

	ClientToScreen(m_windowHandle, &mouse); // convert to entire screen coordinates

	SetCursorPos(mouse.x, mouse.y);
}

void AGN::WindowDX11::showCursor(bool a_shown)
{
	ShowCursor(a_shown);
}

void AGN::WindowDX11::updateWindowState()
{
	RECT clientRect;
	GetClientRect(m_windowHandle, &clientRect);
	int newWidth = clientRect.right - clientRect.left;
	int newHeight = clientRect.bottom - clientRect.top;

	m_dimentions.x = max(newWidth, MINIMUM_WINDOW_SIZE_X);
	m_dimentions.y = max(newHeight, MINIMUM_WINDOW_SIZE_Y);

	m_isDirty = false;

	g_log.info("Resized Window to: %ix%i", m_dimentions.x, m_dimentions.y);
}
