#pragma once

#include <vector>

namespace AGN
{
	class AAssetManager
	{
	public:
		AAssetManager();

		void init();
		class IAMesh& loadModel(std::string a_relativePath);

	private:
		std::vector<class IAMesh*> m_meshList;
		std::vector<class IAShader*> m_shaderList;

	};
}
