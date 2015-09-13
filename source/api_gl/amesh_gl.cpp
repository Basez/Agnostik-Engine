#include "asharedh.hpp"
#include "amesh_gl.hpp"

// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


AGN::AMeshGL::AMeshGL(AMeshData* a_meshData, uint32_t a_vao, uint32_t *a_vbos, uint8_t a_vboCount)
	: m_meshData(a_meshData),
	m_vao(a_vao),
	m_vbos(a_vbos),
	m_vboCount(a_vboCount)
{
	
}

std::string AGN::AMeshGL::getRelativePath()
{
	return m_meshData->relativePath;
}