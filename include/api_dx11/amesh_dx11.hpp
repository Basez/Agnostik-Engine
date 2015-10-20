#pragma once

#include "iamesh.hpp"

struct ID3D11Buffer;

namespace AGN
{
	class AMeshDX11 : public IAMesh
	{
	public:
		AMeshDX11(const uint16_t a_aId, AMeshData* a_meshData, ID3D11Buffer* a_d3d11VertexBuffer, ID3D11Buffer* a_d3d11IndexBuffer)
			: m_aId(a_aId)
			, m_meshData(a_meshData)
			, m_d3d11VertexBuffer(a_d3d11VertexBuffer)
			, m_d3d11IndexBuffer(a_d3d11IndexBuffer)
		{ }
		
		std::string getRelativePath() override { return m_meshData->relativePath; }
		uint16_t getAId() override { return m_aId; }
		class AMaterial* getMaterial() override { return m_meshData->material; }
		glm::vec3 getCenterPoint() override { return m_meshData->centerpoint; } 
		uint32_t getIndexCount() const override { return (uint32_t)m_meshData->indicies.size(); }
		ID3D11Buffer* getD3D11VertexBuffer() const { return m_d3d11VertexBuffer;  }
		ID3D11Buffer* getD3D11IndexBuffer() const { return m_d3d11IndexBuffer; }

		void setMaterial(class AMaterial* a_material) override { m_meshData->material = a_material; }

		struct VertexShaderData
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};

	private:
		const uint16_t m_aId;
		AMeshData* m_meshData;
		ID3D11Buffer* m_d3d11VertexBuffer;
		ID3D11Buffer* m_d3d11IndexBuffer;

	};
}