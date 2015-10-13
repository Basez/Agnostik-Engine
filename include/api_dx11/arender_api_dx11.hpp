#pragma once

#include "iarender_api.hpp"

namespace AGN
{
	class ARenderAPIDX11 : public IARenderAPI
	{
	public:
		ARenderAPIDX11();

		bool init() override;
		void enableVSync(bool a_value) override;
		void handleEvents() override;

		virtual class IAWindow& getWindow() override;
		virtual class IADevice& getDevice() override;
		virtual class IARenderer& getRenderer() override;

	private:
		bool m_initialized;
		class AWindowDX11* m_window;
		class ADeviceDX11* m_device;
		class ARendererDX11* m_renderer;
	};
}
