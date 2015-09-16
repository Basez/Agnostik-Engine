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
		virtual class IAMesh* createMesh(struct AMeshData* a_meshData) = 0;
		virtual class IATexture* createTexture(struct ATextureData* a_textureData) = 0;
		virtual class IAShader* createShader(const char* a_shaderSource, EAShaderType a_type) = 0;
		virtual class IAMaterial* createMaterial(std::string a_name, std::vector<class IAShader*> a_shaders) = 0;
	};

}
