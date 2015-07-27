#pragma once 

namespace AGN
{
	class IARenderer
	{
	public:
		virtual void init() = 0;
		virtual ~IARenderer() {};
	};
}