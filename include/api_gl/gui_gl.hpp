#pragma once

#include "i_gui.hpp"
#include <imgui/imgui.h>
#include <SDL/SDL_events.h> // TODO: refactor this out, find way to remove SDL_EVENT* function

struct SDL_Window;

namespace AGN
{
	class GUIGL : public IGUI
	{
	public:
		bool init(SDL_Window* a_window);
		bool processEvent(SDL_Event* a_event);

		void shutdown() override;
		void update(float a_deltaTime) override;

	private:
		// Use if you want to reset your rendering device without losing ImGui state.
		void invalidateDeviceObjects();
		void createDeviceObjects();
		void createShaders();
		void createImGUIFont();
		SDL_Window* m_window;
	};
}