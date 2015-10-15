#pragma once

namespace AGN
{
	// fwd declare (for enum classes)
	enum class EAShaderType;

	class IADevice
	{
	public:
		virtual ~IADevice() {}

		virtual bool init() = 0;
		virtual class IAMesh* createMesh(const uint16_t a_aId, struct AMeshData* a_meshData) = 0;
		virtual class IATexture* createTexture(const uint16_t a_aId, struct ATextureData* a_textureData) = 0;
		virtual class IAShader* createShader(const uint16_t a_aId, const char* a_shaderSource, EAShaderType a_type) = 0;
		virtual class IAShaderPipeline* createShaderPipeline(const uint16_t a_aId, std::vector<class IAShader*> a_shaders) = 0;
	};

}
