#pragma once

// TODO:
namespace AGN
{
	class IARenderAPI
	{
	public:
		virtual void init() = 0;
		virtual void submitDrawCall() = 0;
		virtual void renderDrawQueue() = 0;
	};
}
