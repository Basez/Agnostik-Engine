#include "asharedh.hpp"
#include "awindow_dx11.hpp"
#include "aosutils.hpp"
#include "iainput.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// accessor for global window procedure callback function
static AGN::AWindowDX11* g_window = nullptr;

// process messages sent to the window.
LRESULT CALLBACK AGN::windowProcedureCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: refactor?
	return g_window->onWindowEvent(hwnd, message, wParam, lParam);
}

AGN::AWindowDX11::AWindowDX11(glm::ivec2 a_dimentions)
	: m_dimentions(a_dimentions)
{
	g_window = this;

	static TCHAR applicationName[] = TEXT("agnostik_dx11");
	static TCHAR windowName[] = TEXT("Agnostik DX11"); // TODO: match openGL window naming style

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &windowProcedureCallback;
	wndClass.hInstance = GetModuleHandle(nullptr);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = applicationName;

	if (!RegisterClassEx(&wndClass))
	{
		g_log.error("Could not register window class");
		return;
	}
	RECT windowRect = { 0, 0, a_dimentions.x, a_dimentions.y };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto moduleHandle = GetModuleHandle(nullptr);

	m_windowHandle = CreateWindowA(applicationName, windowName,
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

// this function was partly inspired by 3dgep.com
LRESULT CALLBACK AGN::AWindowDX11::onWindowEvent(HWND a_hwnd, UINT a_message, WPARAM a_wParam, LPARAM a_lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (a_message)
	{
	case WM_PAINT:
		// erase background
		hDC = BeginPaint(a_hwnd, &paintStruct);
		EndPaint(a_hwnd, &paintStruct);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		//m_isDirty = true; // TODO::?
		break;

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

		uint32_t scanCode = (a_lParam & 0x00FF0000) >> 16;
		uint64_t keyCode = (uint64_t)a_wParam;

		g_log.debug("DX keycode: %i, scancode: %i", keyCode, scanCode);

		// TODO:
		//g_input.registerHold(keyCode, true);

		//KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Pressed, shift, control, alt);
		//pWindow->OnKeyPressed(keyEventArgs);

	}
	break;
	case WM_KEYUP:
	{
		bool shift = GetAsyncKeyState(VK_SHIFT) > 0;
		bool control = GetAsyncKeyState(VK_CONTROL) > 0;
		bool alt = GetAsyncKeyState(VK_MENU) > 0;
		uint32_t scanCode = (a_lParam & 0x00FF0000) >> 16;
		uint64_t keyCode = (uint64_t)a_wParam;

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
		
		// TODO:
		//g_inputManager.setHold(keyCode, false);
	}
	break;

	case WM_MOUSEMOVE:
	{
		bool lButton = (a_wParam & MK_LBUTTON) != 0;
		bool rButton = (a_wParam & MK_RBUTTON) != 0;
		bool mButton = (a_wParam & MK_MBUTTON) != 0;
		bool shift = (a_wParam & MK_SHIFT) != 0;
		bool control = (a_wParam & MK_CONTROL) != 0;

		int mouseX = ((int)(short)LOWORD(a_lParam));
		int mouseY = ((int)(short)HIWORD(a_lParam));

		// TODO:
		//g_inputManager.setMouseXY(mouseX, mouseY);

	}
	break;
	case WM_LBUTTONDOWN:
		//g_inputManager.setMouseHold(1, true); // TODO:
		break;
	case WM_MBUTTONDOWN:
		//g_inputManager.setMouseHold(2, true); // TODO:
		break;
	case WM_RBUTTONDOWN:
		//g_inputManager.setMouseHold(3, true); // TODO:
		break;

		break;
	case WM_LBUTTONUP:
		//g_inputManager.setMouseHold(1, false); // TODO:
		//g_inputManager.mouseClick(1); // TODO:
		break;

	case WM_MBUTTONUP:
		//g_inputManager.setMouseHold(2, false); // TODO:
		//g_inputManager.mouseClick(2); // TODO:
		break;

	case WM_RBUTTONUP:
		//g_inputManager.setMouseHold(3, false); // TODO:
		//g_inputManager.mouseClick(3); // TODO:
		break;

	case WM_MOUSEWHEEL:
	{
		// The distance the mouse wheel is rotated.
		// A positive value indicates the wheel was rotated to the right.
		// A negative value indicates the wheel was rotated to the left.
		float zDelta = ((int)(short)HIWORD(a_wParam)) / (float)WHEEL_DELTA;
		//int x = ((int)(short)LOWORD(a_lParam));
		//int y = ((int)(short)HIWORD(a_lParam));

		// Convert the screen coordinates to client coordinates.
		//POINT clientToScreenPoint;
		//clientToScreenPoint.x = x;
		//clientToScreenPoint.y = y;
		//ScreenToClient(a_hwnd, &clientToScreenPoint);
		//g_inputManager.setMouseScroll((int)zDelta); // TODO:
	}
	break;
	default:
		return DefWindowProc(a_hwnd, a_message, a_wParam, a_lParam);
	}

	return 0;
}

void AGN::AWindowDX11::setTitle(const char* a_title)
{
	// TODO:
}

void AGN::AWindowDX11::showMessageBox(const char* a_title, const char* a_message)
{
	MessageBox(nullptr, TEXT(a_message), TEXT(a_title), MB_OK);
}

void AGN::AWindowDX11::warpCursor(glm::ivec2 a_screenPosition)
{
	POINT mouse;
	mouse.x = a_screenPosition.x;
	mouse.y = a_screenPosition.y;

	ClientToScreen(m_windowHandle, &mouse); // convert to entire screen coordinates

	SetCursorPos(mouse.x, mouse.y);
}

void AGN::AWindowDX11::showCursor(bool a_shown)
{
	ShowCursor(a_shown);
}