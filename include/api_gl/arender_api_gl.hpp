#pragma once

#include "iarender_api.hpp"

namespace AGN
{
	class ARenderAPIGL : public IARenderAPI
	{
	public:
		ARenderAPIGL();

		bool init() override;
		void enableVSync(bool a_value) override;
		bool getVSync() override { return m_vSync; }
		void handleEvents(bool& a_doQuit) override;


		virtual class IAWindow& getWindow() override;
		virtual class IADevice& getDevice() override;
		virtual class IARenderer& getRenderer() override;

		static void getOpenGLError();
		void logAvailableGLExtensions();

	private:
		bool initOpenGL();
		bool initGlew();

		bool m_vSync;
		bool m_initialized;
		class AWindowGL* m_window;
		class ADeviceGL* m_device;
		class ARendererGL* m_renderer;
	};
}
