#pragma once

#include "i_render_api.hpp"

struct ID3D11DeviceChild;

namespace AGN
{
	class RenderAPIDX11 : public IRenderAPI
	{
	public:
		RenderAPIDX11();

		bool init() override;
		void enableVSync(bool a_value) override;
		virtual bool getVSync() { return m_vSync; }
		void handleEvents(bool& a_doQuit) override;
		void logLiveObjects() override;

		virtual class IWindow& getWindow() override;
		virtual class IDevice& getDevice() override;
		virtual class IRenderer& getRenderer() override;

	private:

		bool m_initialized;
		bool m_vSync;
		class WindowDX11* m_window;
		class DeviceDX11* m_device;
		class RendererDX11* m_renderer;
	};
}

