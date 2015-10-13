#pragma once

#include "iatexture.hpp"

namespace AGN
{
	class ATextureDX11 : public IATexture
	{
	public:
		ATextureDX11();
		~ATextureDX11() override;
		
		std::string getRelativePath() override;
		void setTextureParams(unsigned int a_flags = 0) override;
		
		uint16_t getId() override { return 0; } // TODO:

	private:

	};
}