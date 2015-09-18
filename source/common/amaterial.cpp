#include "asharedh.hpp"
#include "amaterial.hpp"

AGN::AMaterial::AMaterial(const uint16_t a_aId, AMaterialData& a_data)
	: m_aId(a_aId)
	, m_name(a_data.name)
	, m_diffuseTexture(a_data.diffuseTexture)
	, m_normalTexture(a_data.normalTexture)
	, m_specularTexture(a_data.specularTexture)
{

}

