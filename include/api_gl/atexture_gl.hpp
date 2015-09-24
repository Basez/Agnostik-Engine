#pragma once

#include "iatexture.hpp"

namespace AGN
{
	class ATextureGL : public IATexture
	{
	public:
		ATextureGL(const uint16_t a_id, ATextureData* a_textureData, GLuint a_glId);
		std::string getRelativePath();

		static GLenum getGlTypeByTextureType(EATextureType a_type);
		void pullBuffer();
		void pushBuffer();
		//void readProperties();
		uint16_t getId() override { return m_id; }
		GLuint getGlId() { return m_glId; }
		GLenum getGlType() { return m_glType; }

	private:
		void setTextureParams(unsigned int a_flags = 0);

		const uint16_t m_id;
		ATextureData* m_textureData;
		const GLuint m_glId;
		GLenum m_glType;
	};
}