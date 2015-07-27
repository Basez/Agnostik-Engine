#pragma once 

#include "iarenderer.hpp"

namespace AGN
{
	class ARendererGL : public IARenderer 
	{
	public:
		ARendererGL();
		void init() override;
	};
}