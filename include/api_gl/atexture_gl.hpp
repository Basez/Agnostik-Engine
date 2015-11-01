#pragma once

#include "iatexture.hpp"

namespace AGN
{
	class ATextureGL : public IATexture
	{
	public:
		ATextureGL(const uint16_t a_id, ATextureData* a_textureData, uint32_t a_glId);
		~ATextureGL() override;

		std::string getRelativePath() override;
		static uint32_t getGlTypeByTextureType(EATextureType a_type);
		void pullBuffer();
		void pushBuffer();
		void setTextureParams(unsigned int a_flags = 0) override;

		//void readProperties();

		uint16_t getId() override { return m_id; }
		uint32_t getGlId() { return m_glId; }
		uint32_t getGlType() { return m_glType; }

	private:

		const uint16_t m_id;
		ATextureData* m_textureData;
		const uint32_t m_glId;
		uint32_t m_glType;
	};
}