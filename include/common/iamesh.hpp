#pragma once

namespace AGN
{
	class IAMesh
	{
	public:
		virtual ~IAMesh() {}
		virtual std::string getRelativePath() = 0;
	};
}