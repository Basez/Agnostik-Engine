#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "mesh_dx11.hpp"

#include <d3d11_1.h>


AGN::MeshDX11::MeshDX11(const uint16_t a_aId, MeshData* a_meshData, ID3D11Buffer* a_d3d11VertexBuffer, ID3D11Buffer* a_d3d11IndexBuffer)
	: m_aId(a_aId)
	, m_meshData(a_meshData)
	, m_d3d11VertexBuffer(a_d3d11VertexBuffer)
	, m_d3d11IndexBuffer(a_d3d11IndexBuffer)
{
}

AGN::MeshDX11::~MeshDX11()
{
	safeRelease(m_d3d11VertexBuffer);
	safeRelease(m_d3d11IndexBuffer);

	delete m_meshData;
}