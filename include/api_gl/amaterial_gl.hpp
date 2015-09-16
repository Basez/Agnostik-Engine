#pragma once

#include "iamaterial.hpp"

namespace AGN
{
	class AMaterialGL : public IAMaterial
	{
	public:
		AMaterialGL(std::string a_name, GLuint a_programID, std::vector<class IAShader*> a_shaders);
		GLint getUniformLocation(const char* a_title);
		GLint getAttribLocation(const char* a_title);
		std::string getName() override { return m_name; };

	private:
		std::string m_name;
		GLuint m_programID;
		std::vector<class AShaderGL*> m_shaders;
	};
}