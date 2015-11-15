#pragma once

#include "i_texture.hpp"

namespace AGN
{
	class TextureGL : public ITexture
	{
	public:
		TextureGL(const uint16_t a_id, TextureData* a_textureData, uint32_t a_glId);
		~TextureGL() override;

		std::string getRelativePath() override;
		static uint32_t getGlTypeByTextureType(ETextureType a_type);
		void pullBuffer();
		void pushBuffer();

		//void readProperties();

		uint16_t getId() override { return m_id; }
		uint32_t getGlId() { return m_glId; }
		uint32_t getGlType() { return m_glType; }

	private:

		const uint16_t m_id;
		TextureData* m_textureData;
		const uint32_t m_glId;
		uint32_t m_glType;
	};
}