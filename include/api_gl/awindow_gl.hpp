#pragma once

#include "iawindow.hpp"

struct SDL_Window;
namespace AGN
{
	class AWindowGL : public IAWindow
	{
	public:
		AWindowGL(glm::ivec2 a_dimentions);

		void setTitle(const char* a_title) override;
		void showMessageBox(const char* a_title, const char* a_message) override;
		void warpCursor(glm::ivec2 a_screenPosition) override;
		void showCursor(bool a_shown) override;
		virtual bool isDirty() override { return m_isDirty; }
		void updateWindowState() override;

		glm::ivec2 getDimentions() override { return m_dimentions; }
		SDL_Window* getSDLWindow() { return m_sdlWindow; }
		void onWindowEvent(SDL_Event a_event);
	
	private:
		
		glm::ivec2 m_dimentions;
		SDL_Window* m_sdlWindow;
		bool m_mouseOnScreen;
		bool m_keyboardFocus;
		bool m_minimized;
		bool m_isDirty;
	};
}