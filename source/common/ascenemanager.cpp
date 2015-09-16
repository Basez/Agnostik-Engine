#include "asharedh.hpp"
#include "ascenemanager.hpp"
#include "iarender_api.hpp"
#include "aresourcemanager.hpp"
#include "aaplication.hpp"
#include "iamesh.hpp"
#include "iatexture.hpp"
#include "iamaterial.hpp"
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

	AMaterialData materialData;
	materialData.name = "meshmaterial";
	materialData.vertexShader = g_shader_mesh_vert;
	materialData.pixelShader = g_shader_mesh_pix;
	IAMaterial& myShaderProgram = g_application.getResourceManager().createMaterial(materialData);

	// TODO: bind textures to shader?

	// create the suzanna entity;
	AEntity* suzanneEntity = new AEntity();
	suzanneEntity->setMesh(&suzanneMesh);
	suzanneEntity->setMaterial(&myShaderProgram);
	suzanneEntity->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(suzanneEntity);
}