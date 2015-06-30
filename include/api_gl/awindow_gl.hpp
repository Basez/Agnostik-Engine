#pragma once

#include "iawindow.hpp"

class SDL_Window;

namespace AGN
{
	class AWindowGL : public IAWindow
	{
	public:
		AWindowGL(glm::ivec2 a_dimentions);

		void setTitle(const char* a_title) override;
		void showMessageBox(const char* a_title, const char* a_message) override;
		
		glm::ivec2 getDimentions() override { return m_dimentions; }
		SDL_Window* getSDLWindow() { return m_sdlWindow; }

	private:
		glm::ivec2 m_dimentions;
		SDL_Window* m_sdlWindow;
	};
}