#pragma once
#include "i_gui.hpp"
#include <imgui/imgui.h>	// TODO: refactor out later

namespace AGN
{
	class GUIDX11 : public IGUI
	{
	public:
		GUIDX11();
		//bool init(SDL_Window* a_window);
		//void processEvent(SDL_Event* a_event);
		void shutdown() override;
		void update(float a_deltaTime) override;
		void render(ImDrawData* draw_data);
		bool isEnabled() override { return m_isEnabled; }

	private:
	
#if defined(__GNUC__) || defined(__GNUG__)
		// GCC doesnt support static friend
		friend void renderDrawLists(ImDrawData* draw_data);
#else
		static friend void renderDrawLists(ImDrawData* draw_data);
#endif

		void invalidateDeviceObjects();
		void createDeviceObjects();
		void createImGUIFont();

		bool m_isEnabled;
	};
}