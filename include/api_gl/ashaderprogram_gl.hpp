#pragma once

#include "iashaderprogram.hpp"

namespace AGN
{
	class AShaderProgramGL : public IAShaderProgram
	{
	public:
		AShaderProgramGL(GLuint a_programID, std::vector<class IAShader*> a_shaders);
		GLint getUniformLocation(const char* a_title);
		GLint getAttribLocation(const char* a_title);

	private:
		GLuint m_programID;
		std::vector<class AShaderGL*> m_shaders;
	};
}