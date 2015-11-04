#pragma once

#include "i_render_api.hpp"

namespace AGN
{
	class RenderAPIGL : public IRenderAPI
	{
	public:
		RenderAPIGL();

		bool init() override;
		void enableVSync(bool a_value) override;
		bool getVSync() override { return m_vSync; }
		void handleEvents(bool& a_doQuit) override;


		virtual class IWindow& getWindow() override;
		virtual class IDevice& getDevice() override;
		virtual class IRenderer& getRenderer() override;
		virtual class IGUI& getGUI() override;

		static void getOpenGLErrors();
		void logAvailableGLExtensions();

	private:
		bool initOpenGL();
		bool initGlew();

		bool m_vSync;
		bool m_initialized;
		class WindowGL* m_window;
		class DeviceGL* m_device;
		class RendererGL* m_renderer;
		class GUIGL* m_gui;
	};
}
