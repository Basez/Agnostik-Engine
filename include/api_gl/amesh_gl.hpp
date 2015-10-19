#pragma once

#include "iamesh.hpp"

namespace AGN
{
	class AMeshGL : public IAMesh
	{
	public:
		AMeshGL(const uint16_t a_aId, const uint32_t a_vao, const uint32_t *a_vbos, const uint8_t a_vboCount, AMeshData* a_meshData)
			: m_aId(a_aId)
			, m_vao(a_vao)
			, m_vbos(a_vbos)
			, m_vboCount(a_vboCount)
			, m_meshData(a_meshData)
		{ };
		
		std::string getRelativePath() override { return m_meshData->relativePath; }
		uint16_t getAId() override { return m_aId; }
		uint32_t getVao() const { return m_vao; }
		uint32_t getIndexCount() const override { return (uint32_t)m_meshData->indicies.size();  }
		class AMaterial* getMaterial() override { return m_meshData->material; }
		glm::vec3 getCenterPoint() override { return m_meshData->centerpoint; }


		void setMaterial(class AMaterial* a_material) override { m_meshData->material = a_material; }

		enum class EAMeshGLAttribute
		{
			MESH_POSITION_ATTRIBUTE = 0,
			MESH_NORMAL_ATTRIBUTE = 1,
			MESH_TANGENT_ATTRIBUTE = 2,
			MESH_BITANGENT_ATTRIBUTE = 3,
			MESH_TEXCOORD_ATTRIBUTE = 4,
		};
	
	private:
		const uint16_t m_aId;
		const uint32_t m_vao;
		const uint32_t *m_vbos;
		const uint8_t m_vboCount;

		AMeshData* m_meshData;
	};
}