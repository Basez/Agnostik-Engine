#include "asharedh.hpp"
#include "aentity.hpp"
#include "iamesh.hpp"
#include "iamaterial.hpp"
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

void AGN::AEntity::setMesh(AGN::IAMesh* a_mesh)
{
	if (m_mesh != nullptr)
	{
		g_log.error("Mesh already set on Entity!");
		return;
	}

	m_mesh = a_mesh;
}

void AGN::AEntity::setMaterial(AGN::IAMaterial* a_material)
{
	if (m_material != nullptr)
	{
		g_log.error("material already set on Entity!");
		return;
	}

	m_material = a_material;
}