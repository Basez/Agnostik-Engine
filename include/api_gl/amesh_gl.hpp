#pragma once

#include "iamesh.hpp"

namespace AGN
{
	class AMeshGL : public IAMesh
	{
	public:
		AMeshGL();
		std::string getRelativePath() override;

	private:
		
	};
}