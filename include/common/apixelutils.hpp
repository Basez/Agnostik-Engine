#define REDMASK		(0xff000000)
#define GREENMASK	(0x00ff0000)
#define BLUEMASK	(0x0000ff00)
#define ALPHAMASK	(0x000000ff)

namespace AGN
{
	namespace PixelUtils
	{
		inline uint32_t addBlend(const uint32_t& a_color1, const uint32_t& a_color2)
		{
			const uint32_t r = (a_color1 & REDMASK) + (a_color2 & REDMASK);
			const uint32_t g = (a_color1 & GREENMASK) + (a_color2 & GREENMASK);
			const uint32_t b = (a_color1 & BLUEMASK) + (a_color2 & BLUEMASK);
			const uint32_t r1 = (r & REDMASK) | (REDMASK * (r >> 24));
			const uint32_t g1 = (g & GREENMASK) | (GREENMASK * (g >> 16));
			const uint32_t b1 = (b & BLUEMASK) | (BLUEMASK * (b >> 8));
			return (r1 + g1 + b1);
		}

		// subtractive blending
		inline uint32_t subBlend(const uint32_t& a_color1, const uint32_t& a_color2)
		{
			uint32_t red = (a_color1 & REDMASK) - (a_color2 & REDMASK);
			uint32_t green = (a_color1 & GREENMASK) - (a_color2 & GREENMASK);
			uint32_t blue = (a_color1 & BLUEMASK) - (a_color2 & BLUEMASK);
			return (uint32_t)(red + green + blue);
		}

		inline void getRGBAFloat(const uint32_t& a_color, float& out_r, float& out_g, float& out_b, float& out_a)
		{
			out_r = float((a_color & REDMASK) >> 24) / 255.0f;
			out_g = float((a_color & GREENMASK) >> 16) / 255.0f;
			out_b = float((a_color & BLUEMASK) >> 8) / 255.0f;
			out_a = float((a_color & ALPHAMASK)) / 255.0f;
		}

		inline uint32_t getUintColorFromFloat(const float& a_red, const float& a_green, const float& a_blue)
		{
			uint8_t red = uint8_t(a_red * 255.0f) << 24;
			uint8_t blue = uint8_t(a_green * 255.0f) << 16;
			uint8_t green = uint8_t(a_blue * 255.0f) << 8;

			return red + blue + green;
		}

		inline float getRedFloat(const uint32_t& a_color)
		{
			return float((a_color & REDMASK) >> 24) / 255.0f;
		}

		inline float getGreenFloat(const uint32_t& a_color)
		{
			return float((a_color & GREENMASK) >> 16) / 255.0f;
		}

		inline float getBlueFloat(const uint32_t& a_color)
		{
			return float((a_color & BLUEMASK) >> 8) / 255.0f;
		}

		inline float getAlphaFloat(const uint32_t& a_color)
		{
			return float(a_color & ALPHAMASK) / 255.0f;
		}

		

	}
}
