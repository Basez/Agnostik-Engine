#include "asharedh.hpp"
#include "amesh_gl.hpp"

// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


AGN::AMeshGL::AMeshGL(uint32_t a_vao, uint32_t *a_vbos, uint8_t a_vboCount)
	: m_vao(a_vao),
	m_vbos(a_vbos),
	m_vboCount(a_vboCount)
{

}

std::string AGN::AMeshGL::getRelativePath()
{
	return "TODO:";
}


