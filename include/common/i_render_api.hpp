#pragma once

namespace AGN
{
	class IRenderAPI
	{
	public:
		virtual ~IRenderAPI() {}

		virtual bool init() = 0;
		virtual void enableVSync(bool a_value) = 0;
		virtual bool getVSync() = 0;
		virtual void handleEvents(bool& a_doQuit) = 0;
		virtual void logLiveObjects() {};

		virtual class IWindow& getWindow() = 0;
		virtual class IDevice& getDevice() = 0;
		virtual class IRenderer& getRenderer() = 0;
		virtual class IGUI& getGUI() = 0;
	};
}
