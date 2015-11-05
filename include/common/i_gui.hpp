#pragma once

namespace AGN
{
	class IGUI
	{
	public:
		virtual ~IGUI() {}
		virtual void shutdown() = 0;
		virtual void update(float a_deltaTime) = 0;
		virtual bool isEnabled() = 0;
	};
}