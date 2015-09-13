#pragma once

#include <vector>

namespace AGN
{
	enum class EATextureType;

	class AAssetManager
	{
	public:
		AAssetManager();

		void init();
		class IAMesh& loadMesh(std::string a_relativePath, uint32_t additional_assimp_flags = 0);
		class IATexture& loadTexture(std::string a_relativePath, EATextureType a_textureType);

	private:
		std::vector<class IAMesh*> m_meshList;
		std::vector<class IAShader*> m_shaderList;
		std::vector<class IATexture*> m_textureList;
	};
}
