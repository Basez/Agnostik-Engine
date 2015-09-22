#pragma once

#include "iamesh.hpp"

namespace AGN
{
	class AMeshGL : public IAMesh
	{
	public:
		AMeshGL(const uint16_t a_aId, const uint32_t a_vao, const uint32_t *a_vbos, const uint8_t a_vboCount, AMeshData* a_meshData);
		
		std::string getRelativePath() override;
		uint16_t getAId() override { return m_aId; }
		const uint32_t getVao() const { return m_vao; }
		const uint32_t getIndexCount() const { return (uint32_t)m_meshData->indicies.size();  }

		enum class EAMeshGLAttribute
		{
			MESH_POSITION_ATTRIBUTE = 0,
			MESH_NORMAL_ATTRIBUTE = 2,
			MESH_DIFFUSE_ATTRIBUTE = 3,
			MESH_SPECULAR_ATTRIBUTE = 4,
			MESH_TEXCOORD_ATTRIBUTE = 8
		};
	
	private:
		const uint16_t m_aId;
		const uint32_t m_vao;
		const uint32_t *m_vbos;
		const uint8_t m_vboCount;

		AMeshData* m_meshData;

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