#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "mesh_gl.hpp"

AGN::MeshGL::MeshGL(const uint16_t a_aId, const uint32_t a_vao, const uint32_t *a_vbos, const uint8_t a_vboCount, MeshData* a_meshData)
	: m_aId(a_aId)
	, m_vao(a_vao)
	, m_vbos(a_vbos)
	, m_vboCount(a_vboCount)
	, m_meshData(a_meshData)
{

}

AGN::MeshGL::~MeshGL()
{
	delete m_meshData;
	delete[] m_vbos;
}
