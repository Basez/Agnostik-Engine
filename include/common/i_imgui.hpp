#pragma once

namespace AGN
{
	class IImGui
	{
	public:
		virtual ~IImGui() {}
		virtual void shutdown() = 0;
		virtual void update(float a_deltaTime) = 0;
		virtual bool isEnabled() = 0;
		virtual void setEnabled(bool a_value) = 0;
	};
}