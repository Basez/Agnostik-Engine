#pragma once

#include "iatexture.hpp"

namespace AGN
{
	class ATextureGL : public IATexture
	{
	public:
		ATextureGL(ATextureData* a_textureData, GLenum a_textureID);
		std::string getRelativePath();

		static GLenum getGlType(EATextureType a_type);
		void pullBuffer();
		void pushBuffer();
		void readProperties();

	private:
		void setTextureParams(unsigned int a_flags);

		ATextureData* m_textureData;
		const GLenum m_textureID;
		GLenum m_glType;
		

	};
}