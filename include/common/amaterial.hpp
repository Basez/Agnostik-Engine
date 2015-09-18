#pragma once

#include "amaterial.hpp"

namespace AGN
{
	class AMaterial
	{
	public:
		AMaterial(const uint16_t a_aId, struct AMaterialData& a_data);
		
		// TODO: get properties material specifc?
		//GLint getUniformLocation(const char* a_title);
		//GLint getAttribLocation(const char* a_title);
		
		std::string getName() { return m_name; };
		uint16_t getAId() { return m_aId; }
		class IATexture* getDiffuseTexture() { return m_diffuseTexture; }
		class IATexture* getNormalTexture() { return m_normalTexture; }
		class IATexture* getSpecularTexture() { return m_specularTexture; }

	private:
		const uint16_t m_aId;
		const std::string m_name;
		class IATexture* m_diffuseTexture;
		class IATexture* m_normalTexture;
		class IATexture* m_specularTexture;
	};

	struct AMaterialData
	{
		AMaterialData()
			: name("")
			, diffuseTexture(nullptr)
			, normalTexture(nullptr)
			, specularTexture(nullptr)
		{}

		std::string name; // required
		class IATexture* diffuseTexture; // required
		class IATexture* normalTexture;
		class IATexture* specularTexture;
	};

	
}