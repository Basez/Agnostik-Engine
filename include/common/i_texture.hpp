#pragma once

namespace AGN
{
	class ITexture
	{
	public:
		virtual ~ITexture() {}
		virtual std::string getRelativePath() = 0;
		virtual uint16_t getId() = 0;
	};

	enum class ETextureType
	{
		TEXTURE_1D,		// TODO: implement
		TEXTURE_2D,
		CUBEMAP			// TODO: implement
	};

	enum class ETextureRenderFlags
	{
		USE_NEAREST_NEIGBOR = 1,		// renders the texture using nearest neighbor (default is mipmaps)
		USE_LINEAR = 2,					// renders linear (default is mipmaps)
		USE_CLAMP = 4,					// renders using clamp (DEFAULT IS REPEAT)
	};

	struct TextureData
	{
		TextureData()
			: width(0)
			, height(0)
			, components(0)
			, buffer(nullptr)
			, flags(0) {}
			
		~TextureData()
		{
			if (buffer != nullptr) delete[] buffer;
		}

		std::string relativePath;
		ETextureType type;
		int width;
		int height;
		int components;
		uint32_t* buffer;
		uint32_t flags;
	};


}