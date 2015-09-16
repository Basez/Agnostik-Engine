#pragma once

namespace AGN
{
	class IATexture
	{
	public:
		virtual ~IATexture() {}
		virtual std::string getRelativePath() = 0;
		virtual uint16_t getId() = 0;
	};

	enum class EATextureType
	{
		TEXTURE_1D,		// TODO: implement
		TEXTURE_2D,
		CUBEMAP			// TODO: implement
	};

	enum class EATextureRenderFlags
	{
		USE_NEAREST_NEIGBOR = 1,			// renders the texture using nearest neighbor (DEFAULT IS LINEAR)
		USE_MIP_MAPS = 2,					// renders using mipmaps + mipmap interpolation (DEFAULT IS LINEAR)
		USE_CLAMP = 4,						// renders using clamp (DEFAULT IS REPEAT)
	};

	struct ATextureData
	{
		std::string relativePath;
		EATextureType type; 
		int width;
		int height;
		int components;
		uint32_t* buffer;
		unsigned int flags;
	};


}