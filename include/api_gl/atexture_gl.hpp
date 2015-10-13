#pragma once

#include "iatexture.hpp"

namespace AGN
{
	class ATextureGL : public IATexture
	{
	public:
		ATextureGL(const uint16_t a_id, ATextureData* a_textureData, GLuint a_glId);
		~ATextureGL() override;

		std::string getRelativePath() override;
		static GLenum getGlTypeByTextureType(EATextureType a_type);
		void pullBuffer();
		void pushBuffer();
		void setTextureParams(unsigned int a_flags = 0) override;

		//void readProperties();

		uint16_t getId() override { return m_id; }
		GLuint getGlId() { return m_glId; }
		GLenum getGlType() { return m_glType; }

	private:

		const uint16_t m_id;
		ATextureData* m_textureData;
		const GLuint m_glId;
		GLenum m_glType;
	};
}