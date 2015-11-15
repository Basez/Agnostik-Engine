#pragma once

namespace AGN
{
	class IRenderAPI
	{
	public:
		virtual ~IRenderAPI() {}

		virtual bool init() = 0;
		virtual void handleEvents(bool& a_doQuit) = 0;
		virtual void updateWindowState() = 0;
		virtual void logLiveObjects() {};

		// setters / getters
		virtual void enableVSync(bool a_value) = 0;
		virtual void enableNormalMapping(bool a_value) = 0;
		virtual void enableSpecularMapping(bool a_value) = 0;
		virtual class IWindow& getWindow() = 0;
		virtual class IDevice& getDevice() = 0;
		virtual class IRenderer& getRenderer() = 0;
		virtual class IImGui& getImGui() = 0;
		virtual bool getVSync() = 0;
	};
}
