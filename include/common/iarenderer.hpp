#pragma once 

namespace AGN
{
	class IARenderer
	{
	public:
		virtual void init() = 0;
		virtual void render(class ADrawCommander& a_drawCommander) = 0;
		virtual ~IARenderer() {};
	};
}