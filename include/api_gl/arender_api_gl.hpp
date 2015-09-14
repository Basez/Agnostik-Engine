#pragma once

#include "iarender_api.hpp"

namespace AGN
{
	class ARenderAPIGL : public IARenderAPI
	{
	public:
		ARenderAPIGL();

		void init() override;
		void submitDrawCall() override;
		void renderDrawQueue() override;
		void enableVSync(bool a_value) override;

		virtual class IAWindow& getWindow() override;
		virtual class IADevice& getDevice() override;
		virtual class IARenderer& getRenderer() override;

		void logAvailableGLExtensions();

	private:
		void initOpenGL();
		void initGlew();

		bool m_initialized;
		class AWindowGL* m_window;
		class ADeviceGL* m_device;
		class ARendererGL* m_renderer;
	};
}
