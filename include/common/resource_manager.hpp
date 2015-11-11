#pragma once

namespace AGN
{
	// fwd declare enum classes
	enum class ETextureType;
	enum class EShaderType;

	typedef std::vector<class IMesh*> MeshCollection;

	class ResourceManager
	{
	public:
		ResourceManager(class IDevice& a_device);
		~ResourceManager();

		void loadDefaults();
		
		class std::vector<class IMesh*> loadMeshCollection(std::string a_relativePath, uint32_t additional_assimp_flags = 0);
		//class std::vector<class IAMesh*> loadMaterial(std::string a_relativePath);

		class ITexture& loadTexture(std::string a_relativePath, ETextureType a_textureType);

		class IShader& createShader(const char* a_shaderSource, EShaderType a_shaderType);
		class IShaderPipeline& createShaderPipeline(std::vector<class IShader*> a_shaders);
		class Material& createMaterial(std::string a_name);

	private:
		std::vector<MeshCollection> m_loadedMeshCollections;
		std::vector<class ITexture*> m_loadedTextures;
		std::vector<class IShader*> m_shaders;
		std::vector<class IShaderPipeline*> m_shaderpipelines;
		std::vector<class Material*> m_materials;


		class IDevice& m_device;
		uint16_t m_meshIdCount;
		uint16_t m_shaderIdCount;
		uint16_t m_textureIdCount;
		uint16_t m_materialIdCount;
		uint16_t m_shaderPipelineIdCount;

		class Material* m_defaultMaterial;
	};
}
