#pragma once

#include "i_window.hpp"
#include <basetsd.h>

// fwd declare
// TODO: review if these are smart fwd declares as they are pretty much ripped from a windows header
typedef struct HWND__ *HWND;
#define CALLBACK __stdcall
typedef LONG_PTR LRESULT;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef unsigned int UINT;


namespace AGN
{
	class WindowDX11 : public IWindow
	{
	public:
		WindowDX11(glm::ivec2 a_dimentions);
		~WindowDX11() override;

		void setTitle(const char* a_title) override;
		void showMessageBox(const char* a_title, const char* a_message) override;
		void warpCursor(glm::ivec2 a_screenPosition) override;
		void showCursor(bool a_shown) override;
		void updateWindowState() override;
		bool isDirty() override { return m_isDirty; }
		glm::ivec2 getDimentions() override { return m_dimentions; }
		const HWND getWindowHandle() const { return m_windowHandle; }

	private:
		glm::ivec2 m_dimentions;
		HWND m_windowHandle = 0;
		bool m_isDirty;
		std::string m_windowName;
		std::string m_applicationName;

		// TODO: check if friend function is necessary
		LRESULT CALLBACK onWindowEvent(HWND a_hwnd, UINT a_message, WPARAM a_wParam, LPARAM a_lParam);
		friend LRESULT CALLBACK windowProcedureCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}