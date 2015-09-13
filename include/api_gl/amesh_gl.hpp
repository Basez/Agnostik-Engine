#pragma once

#include "iamesh.hpp"

namespace AGN
{
	class AMeshGL : public IAMesh
	{
	public:
		AMeshGL(AMeshData* a_meshData, uint32_t a_vao, uint32_t *a_vbos, uint8_t a_vboCount);
		std::string getRelativePath() override;

		enum EAMeshGLAttribute
		{
			MESH_POSITION_ATTRIBUTE = 0,
			MESH_NORMAL_ATTRIBUTE = 2,
			MESH_DIFFUSE_ATTRIBUTE = 3,
			MESH_SPECULAR_ATTRIBUTE = 4,
			MESH_TEXCOORD_ATTRIBUTE = 8
		};
	
	private:
		AMeshData* m_meshData;
		uint32_t m_vao;
		uint32_t *m_vbos;
		uint8_t m_vboCount;

		//class Texture* m_textureDiffuse;
		//class Texture* m_textureNormal;
		//class Texture* m_textureSpecular;

		/////////////////////
		// shaders
		//class Shader* m_shaderProgram;

		// Model, View, Projection matrix uniform variable in shader program.
		//GLint m_uniformMVP;
		//GLint m_uniformColor;
		//GLint m_uniformModelMatrix;
		//GLint m_uniformClipPlane;
		//GLint m_uniformClipOffset;

		//GLint m_uniformLightDirection;
		//GLint m_uniformLightColor;
		//GLint m_uniformAmbient;
		//GLint m_uniformMaterialEmissive;
		//GLint m_uniformMaterialDiffuse;
	};
}