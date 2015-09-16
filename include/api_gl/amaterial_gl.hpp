#pragma once

#include "iamaterial.hpp"

namespace AGN
{
	class AMaterialGL : public IAMaterial
	{
	public:
		AMaterialGL(const uint16_t a_id, std::string a_name, GLuint a_programID, std::vector<class IAShader*> a_shaders);
		GLint getUniformLocation(const char* a_title);
		GLint getAttribLocation(const char* a_title);
		std::string getName() override { return m_name; };
		uint16_t getId() override { return m_id; }

	private:
		const uint16_t m_id;
		std::string m_name;
		GLuint m_programID;
		std::vector<class AShaderGL*> m_shaders;
	};
}