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

		virtual class IAWindow& getWindow() override;
		virtual class IADevice& getDevice() override;
		virtual class IARenderer& getRenderer() override;

	private:
		void initOpenGL();
		void initGlew();

		class AWindowGL* m_window;
		class ADeviceGL* m_device;
		class ARendererGL* m_renderer;
	};
}
