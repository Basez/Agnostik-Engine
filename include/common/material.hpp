#pragma once

#include "material.hpp"

namespace AGN
{
	class Material
	{
	public:
		Material(const uint16_t a_aId, std::string a_name)
			: m_aId(a_aId)
			, m_name(a_name)
			, diffuseTexture(nullptr)
			, normalTexture(nullptr)
			, specularTexture(nullptr)
			, diffuseColor(glm::vec3(1.0f))
			, specularColor(glm::vec3(0.0f))
			, ambientColor(glm::vec3(0.0f))
			, transparency(0.0f)
			, specularPower(0.0f)
		{
		}

	private:
		const uint16_t m_aId;
		const std::string m_name;

	public:
		std::string getName() { return m_name; };
		uint16_t getAId() { return m_aId; }

		class ITexture* diffuseTexture;
		class ITexture* normalTexture;
		class ITexture* specularTexture;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		glm::vec3 ambientColor;
		float specularPower;
		float transparency;


	};
}