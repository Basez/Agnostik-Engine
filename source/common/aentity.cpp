#include "asharedh.hpp"
#include "aentity.hpp"
#include "iamesh.hpp"
#include "amaterial.hpp"
#include "iatexture.hpp"

using namespace glm;

AGN::AEntity::AEntity()
	: m_mesh(nullptr)
	, m_material(nullptr)
	, m_position(glm::vec3())
	, m_rotation(glm::quat())
	, m_scale(glm::vec3(1))
{

}