#include "asharedh.hpp"
#include "amesh_gl.hpp"

AGN::AMeshGL::AMeshGL(const uint16_t a_id, const uint32_t a_vao, const uint32_t *a_vbos, const uint8_t a_vboCount, AMeshData* a_meshData)
	: m_id(a_id)
	, m_vao(a_vao)
	, m_vbos(a_vbos)
	, m_vboCount(a_vboCount)
	, m_meshData(a_meshData)
{
	
}

std::string AGN::AMeshGL::getRelativePath()
{
	return m_meshData->relativePath;
}