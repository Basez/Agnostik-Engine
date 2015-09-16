#pragma once

namespace AGN
{
	// fwd declare enum classes
	enum class EATextureType;
	enum class EAShaderType;

	class AResourceManager
	{
	public:
		AResourceManager();

		void init();
		class IAMesh& loadMesh(std::string a_relativePath, uint32_t additional_assimp_flags = 0);
		class IATexture& loadTexture(std::string a_relativePath, EATextureType a_textureType);
		class IAMaterial& createMaterial(struct AMaterialData& a_data);

	private:
		std::vector<class IAMesh*> m_meshList;
		std::vector<class IAMaterial*> m_materialList;
		std::vector<class IATexture*> m_textureList;
	};
}
