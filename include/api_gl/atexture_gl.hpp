#pragma once

#include "iatexture.hpp"

namespace AGN
{
	class ATextureGL : public IATexture
	{
	public:
		ATextureGL(const uint16_t a_id, ATextureData* a_textureData, GLenum a_textureID);
		std::string getRelativePath();

		static GLenum getGlType(EATextureType a_type);
		void pullBuffer();
		void pushBuffer();
		void readProperties();
		uint16_t getId() override { return m_id; }

	private:
		void setTextureParams(unsigned int a_flags);

		const uint16_t m_id;
		ATextureData* m_textureData;
		const GLenum m_textureID;
		GLenum m_glType;
		

	};
}