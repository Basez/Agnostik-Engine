#pragma once

namespace AGN
{
	// fwd declare (for enum classes)
	enum class EAShaderType;

	class IADevice
	{
	public:
		virtual ~IADevice() {}

		virtual void init() = 0;
		virtual class IAMesh* createMesh(const uint16_t a_id, struct AMeshData* a_meshData) = 0;
		virtual class IATexture* createTexture(const uint16_t a_id, struct ATextureData* a_textureData) = 0;
		virtual class IAShader* createShader(const char* a_shaderSource, EAShaderType a_type) = 0;
		virtual class IAMaterial* createMaterial(const uint16_t a_id, std::string a_name, std::vector<class IAShader*> a_shaders) = 0;
	};

}
