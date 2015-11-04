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
		GUIGL();
		bool init(SDL_Window* a_window);
		void processEvent(SDL_Event* a_event);
		void shutdown() override;
		void update(float a_deltaTime) override;
		void render(ImDrawData* draw_data);
		
	private:
		friend void renderDrawLists(ImDrawData* draw_data);

		void invalidateDeviceObjects();
		void createDeviceObjects();
		void createImGUIFont();

		SDL_Window* m_window;
		int32_t m_shaderHandle;
		int32_t m_vertHandle;
		int32_t m_fragHandle;
		int32_t m_attribLocationTex;
		int32_t m_attribLocationProjMtx;
		int32_t m_attribLocationPosition;
		int32_t m_attribLocationUV;
		int32_t m_attribLocationColor;
		uint32_t m_vboHandle;
		uint32_t m_vaoHandle;
		uint32_t m_elementsHandle;
		uint32_t m_fontTexture;

	};
}