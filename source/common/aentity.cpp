#include "asharedh.hpp"
#include "aentity.hpp"
#include "iamesh.hpp"
#include "iashaderprogram.hpp"
#include "iatexture.hpp"

using namespace glm;

AGN::AEntity::AEntity()
	: m_mesh(nullptr)
	, m_shaderProgram(nullptr)
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

void AGN::AEntity::setShaderProgram(AGN::IAShaderProgram* a_shaderProgram)
{
	if (m_shaderProgram != nullptr)
	{
		g_log.error("Mesh already set on Entity!");
		return;
	}

	m_shaderProgram = a_shaderProgram;
}