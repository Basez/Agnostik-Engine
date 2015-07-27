#pragma once

#include "iaassetmanager.hpp"

namespace AGN
{
	class AAssetManagerGL : public IAAssetManager
	{
	public:
		AAssetManagerGL();

		void init() override;
	};
}
