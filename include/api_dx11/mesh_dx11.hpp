#pragma once

#include "i_mesh.hpp"

struct ID3D11Buffer;

namespace AGN
{
	class MeshDX11 : public IMesh
	{
	public:
		MeshDX11(const uint16_t a_aId, MeshData* a_meshData, ID3D11Buffer* a_d3d11VertexBuffer, ID3D11Buffer* a_d3d11IndexBuffer);
		~MeshDX11();

		std::string getRelativePath() override { return m_meshData->relativePath; }
		uint16_t getAId() override { return m_aId; }
		class Material* getMaterial() override { return m_meshData->material; }
		glm::vec3 getCenterPoint() override { return m_meshData->centerpoint; } 
		uint32_t getIndexCount() const override { return (uint32_t)m_meshData->indicies.size(); }
		ID3D11Buffer* getD3D11VertexBuffer() const { return m_d3d11VertexBuffer;  }
		ID3D11Buffer* getD3D11IndexBuffer() const { return m_d3d11IndexBuffer; }
		std::string getName() override { return m_meshData->name; }

		void setMaterial(class Material* a_material) override { m_meshData->material = a_material; }

		struct VertexShaderData
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 textureCoords;
			glm::vec3 tangent;
			glm::vec3 biTangent;
		};

	private:
		const uint16_t m_aId;
		MeshData* m_meshData;
		ID3D11Buffer* m_d3d11VertexBuffer;
		ID3D11Buffer* m_d3d11IndexBuffer;

	};



}
