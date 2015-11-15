#pragma once

#include "i_imgui.hpp"
struct ImDrawData;
struct SDL_Window;

namespace AGN
{
	class ImGuiGL : public IImGui
	{
	public:
		ImGuiGL();
		~ImGuiGL() override;

		bool init(SDL_Window* a_window);
		void shutdown() override;
		void update(float a_deltaTime) override;
		void render(ImDrawData* draw_data);
		bool isEnabled() override { return m_isEnabled; }
		void setEnabled(bool a_value) override { m_isEnabled = a_value; }

	private:
	
#if defined(__GNUC__) || defined(__GNUG__)
		// GCC doesnt support static friend functions
		friend void renderDrawLists(ImDrawData* draw_data);
#else
		static friend void renderDrawLists(ImDrawData* draw_data);
#endif

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
		int32_t m_attribLocationTextureCoords;
		int32_t m_attribLocationColor;
		uint32_t m_vboHandle;
		uint32_t m_vaoHandle;
		uint32_t m_elementsHandle;
		uint32_t m_fontTexture;
		bool m_isEnabled;
	};
}