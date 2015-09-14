#include "asharedh.hpp"
#include "ascenemanager.hpp"
#include "iarender_api.hpp"
#include "aresourcemanager.hpp"
#include "aaplication.hpp"
#include "iamesh.hpp"
#include "iatexture.hpp"
#include "iashaderprogram.hpp"
#include "aentity.hpp"
#include "acamera.hpp"

// shaders
#include "shader_mesh_pix.hpp"
#include "shader_mesh_vert.hpp"

AGN::ASceneManager::ASceneManager()
{
	
}

AGN::ASceneManager::~ASceneManager()
{
	
}

void AGN::ASceneManager::init()
{
	m_camera = new ACamera();
	m_camera->setPosition(glm::vec3(0, 10, 0));
	m_camera->setProjectionRH(60.0f, 0.1f, 10000.0f);
}

void AGN::ASceneManager::loadTestScene01()
{
	IAMesh& suzanneMesh = g_application.getResourceManager().loadMesh("suzanne.obj");

	IATexture& myTexture = g_application.getResourceManager().loadTexture("test.png", EATextureType::TEXTURE_2D);

	AShaderProgramData shaderData;
	shaderData.vertexShader = g_shader_mesh_vert;
	shaderData.pixelShader = g_shader_mesh_pix;
	IAShaderProgram& myShaderProgram = g_application.getResourceManager().loadShaderProgram(shaderData);

	// create the suzanna entity;
	AEntity* suzanneEntity = new AEntity();
	suzanneEntity->setMesh(&suzanneMesh);
	suzanneEntity->setShaderProgram(&myShaderProgram);
	suzanneEntity->addTexture(&myTexture);
	suzanneEntity->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(suzanneEntity);
}