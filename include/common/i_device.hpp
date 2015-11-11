#pragma once

namespace AGN
{
	// fwd declare (for enum classes)
	enum class EShaderType;

	class IDevice
	{
	public:
		virtual ~IDevice() {}
		virtual class IMesh* createMesh(const uint16_t a_aId, struct MeshData* a_meshData) = 0;
		virtual class ITexture* createTexture(const uint16_t a_aId, struct TextureData* a_textureData) = 0;
		virtual class IShader* createShader(const uint16_t a_aId, const char* a_shaderSource, EShaderType a_type) = 0;
		virtual class IShaderPipeline* createShaderPipeline(const uint16_t a_aId, std::vector<class IShader*> a_shaders) = 0;
	};

}
