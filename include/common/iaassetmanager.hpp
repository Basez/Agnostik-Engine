#pragma once

namespace AGN
{
	class IAAssetManager
	{
	public:
		virtual void init() = 0;
		virtual ~IAAssetManager() {}
	};
}