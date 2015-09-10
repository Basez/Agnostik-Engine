#pragma once

#include <vector>

namespace AGN
{
	class AAssetManager
	{
	public:
		AAssetManager();

		void init();
		class IAMesh& loadMesh(std::string a_relativePath, uint32_t additional_assimp_flags = 0);

	private:
		std::vector<class IAMesh*> m_meshList;
		std::vector<class IAShader*> m_shaderList;

	};
}
