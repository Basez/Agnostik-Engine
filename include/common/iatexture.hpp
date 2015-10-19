#pragma once

namespace AGN
{
	class IATexture
	{
	public:
		virtual ~IATexture() {}
		virtual std::string getRelativePath() = 0;
		virtual uint16_t getId() = 0;
		virtual void setTextureParams(unsigned int a_flags = 0) = 0;

		// TODO: push/pull buffers for CPU access to texture data
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
		ATextureData()
			: width(0)
			, height(0)
			, components(0)
			, buffer(nullptr)
			, flags(0) {}
			
		~ATextureData()
		{
			if (buffer != nullptr) delete buffer;
		}

		std::string relativePath;
		EATextureType type; 
		int width;
		int height;
		int components;
		uint32_t* buffer;
		unsigned int flags;
	};


}