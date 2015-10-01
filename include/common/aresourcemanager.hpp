#pragma once

namespace AGN
{
	// fwd declare enum classes
	enum class EATextureType;
	enum class EAShaderType;

	class AResourceManager
	{
	public:
		AResourceManager(class IADevice& a_device);

		void loadDefaults();
		
		class IAMesh& loadMesh(std::string a_relativePath, uint32_t additional_assimp_flags = 0);
		class std::vector<class IAMesh*> loadMeshCollection(std::string a_relativePath, uint32_t additional_assimp_flags = 0);
		class IATexture& loadTexture(std::string a_relativePath, EATextureType a_textureType);

		class IAShader& createShader(const char* a_shaderSource, EAShaderType a_shaderType);
		class IAShaderPipeline& createShaderPipeline(std::vector<class IAShader*> a_shaders);
		class AMaterial& createMaterial(struct AMaterialData& a_data);

	private:
		std::vector<class IAMesh*> m_loadedMeshes;
		std::vector<class IATexture*> m_loadedTextures;
		std::vector<class IAShader*> m_shaders;
		std::vector<class IAShaderPipeline*> m_shaderpipelines;
		std::vector<class AMaterial*> m_materials;

		class IADevice& m_device;
		uint16_t m_meshIdCount;
		uint16_t m_shaderIdCount;
		uint16_t m_textureIdCount;
		uint16_t m_materialIdCount;
		uint16_t m_shaderPipelineIdCount;

		class AMaterial* m_defaultMaterial;
	};
}
