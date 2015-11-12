#pragma once

#include "i_mesh.hpp"

namespace AGN
{
	class MeshGL : public IMesh
	{
	public:
		MeshGL(const uint16_t a_aId, const uint32_t a_vao, const uint32_t *a_vbos, const uint8_t a_vboCount, MeshData* a_meshData);
		~MeshGL();
		
		std::string getRelativePath() override { return m_meshData->relativePath; }
		uint16_t getAId() override { return m_aId; }
		uint32_t getVao() const { return m_vao; }
		uint32_t getIndexCount() const override { return (uint32_t)m_meshData->indicies.size();  }
		class Material* getMaterial() override { return m_meshData->material; }
		glm::vec3 getCenterPoint() override { return m_meshData->centerpoint; }
		std::string getName() override { return m_meshData->name; }

		void setMaterial(class Material* a_material) override { m_meshData->material = a_material; }

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

		MeshData* m_meshData;
	};



}