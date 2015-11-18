#pragma once

#include "i_window.hpp"
#include <basetsd.h>

// fwd declare
// TODO: review if these are smart fwd declares as they are pretty much ripped from a windows header
typedef struct HWND__ *HWND;
//#define CALLBACK __stdcall
typedef LONG_PTR LRESULT;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;


namespace AGN
{
	class WindowDX11 : public IWindow
	{
	public:
		WindowDX11(glm::ivec2 a_dimensions);
		~WindowDX11() override;

		void setTitle(const char* a_title) override;
		void showMessageBox(const char* a_title, const char* a_message) override;
		void warpCursor(glm::ivec2 a_screenPosition) override;
		void showCursor(bool a_shown) override;
		void updateWindowState() override;
		bool isDirty() override { return m_isDirty; }
		glm::ivec2 getDimensions() override { return m_dimensions; }
		const HWND getWindowHandle() const { return m_windowHandle; }

	private:
		glm::ivec2 m_dimensions;
		HWND m_windowHandle = 0;
		bool m_isDirty;
		std::string m_windowName;
		std::string m_applicationName;

		// TODO: check if friend function is necessary
		LRESULT __stdcall onWindowEvent(HWND a_hwnd, unsigned int a_message, WPARAM a_wParam, LPARAM a_lParam);
		friend LRESULT __stdcall windowProcedureCallback(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
	};
}