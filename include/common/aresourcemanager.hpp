#pragma once

#include <vector>

namespace AGN
{
	enum class EATextureType;

	class AResourceManager
	{
	public:
		AResourceManager();

		void init();
		class IAMesh& loadMesh(std::string a_relativePath, uint32_t additional_assimp_flags = 0);
		class IATexture& loadTexture(std::string a_relativePath, EATextureType a_textureType);
		class IAShaderProgram& loadShaderProgram(struct AShaderProgramData& a_data);

	private:
		std::vector<class IAMesh*> m_meshList;
		std::vector<class IAShader*> m_shaderList;
		std::vector<class IAShaderProgram*> m_shaderProgramList;
		std::vector<class IATexture*> m_textureList;
	};
}
