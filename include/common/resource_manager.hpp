#pragma once

namespace AGN
{
	// fwd declare enum classes
	enum class ETextureType;
	enum class EShaderType;
	enum class ETextureRenderFlags;

	class ResourceManager
	{
	public:
		ResourceManager(class IDevice& a_device);
		~ResourceManager();

		void unloadAll();
		void loadDefaults();
		
		class MeshCollection& loadMeshCollection(std::string a_relativePath, uint32_t additional_assimp_flags = 0, float a_scaleModifier = 1.0f);
		class ITexture& loadTexture(std::string a_relativePath, ETextureType a_textureType, uint32_t a_textureRenderFlags = 0);
		class IShader& createShader(const char* a_shaderSource, EShaderType a_shaderType);
		class IShaderPipeline& createShaderPipeline(std::vector<class IShader*> a_shaders);
		class Material& createMaterial(std::string a_name);

	private:
		std::vector<class MeshCollection*> m_loadedMeshCollections;
		std::vector<class ITexture*> m_loadedTextures;
		std::vector<class IShader*> m_shaders;
		std::vector<class IShaderPipeline*> m_shaderpipelines;
		std::vector<class Material*> m_materials;


		class IDevice& m_device;
		uint16_t m_meshIdCount;
		uint16_t m_meshCollectionIdCount;
		uint16_t m_shaderIdCount;
		uint16_t m_textureIdCount;
		uint16_t m_materialIdCount;
		uint16_t m_shaderPipelineIdCount;

		class Material* m_defaultMaterial;
	};
}
