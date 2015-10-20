#pragma once

namespace AGN
{
	class IARenderAPI
	{
	public:
		virtual ~IARenderAPI() {}

		virtual bool init() = 0;
		virtual void enableVSync(bool a_value) = 0;
		virtual bool getVSync() = 0;
		virtual void handleEvents(bool& a_doQuit) = 0;
		virtual void logLiveObjects() {};

		virtual class IAWindow& getWindow() = 0;
		virtual class IADevice& getDevice() = 0;
		virtual class IARenderer& getRenderer() = 0;
	};
}
