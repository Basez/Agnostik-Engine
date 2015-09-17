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
	IAMesh& triangleMesh = g_application.getResourceManager().loadMesh("triangle.obj");
	IAMesh& cubeMesh = g_application.getResourceManager().loadMesh("cube.obj");
	IAMesh& skyboxMesh = g_application.getResourceManager().loadMesh("skybox.obj");		

	// TODO: bind textures to shader?
	IATexture& myTexture = g_application.getResourceManager().loadTexture("test.png", EATextureType::TEXTURE_2D);

	AMaterialData materialData;
	materialData.name = "meshmaterial";
	materialData.vertexShader = g_shader_mesh_vert;
	materialData.pixelShader = g_shader_mesh_pix;
	IAMaterial& myMaterial = g_application.getResourceManager().loadMaterial(materialData);

	// create the triangle I entity;
	AEntity* triangleEntityA = new AEntity();
	triangleEntityA->setMesh(&triangleMesh);
	triangleEntityA->setMaterial(&myMaterial);
	triangleEntityA->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(triangleEntityA);

	// create the suzanna entity;
	AEntity* suzanneEntity = new AEntity();
	suzanneEntity->setMesh(&suzanneMesh);
	suzanneEntity->setMaterial(&myMaterial);
	suzanneEntity->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(suzanneEntity);

	// create the cube entity;
	AEntity* cubeEntity = new AEntity();
	cubeEntity->setMesh(&cubeMesh);
	cubeEntity->setMaterial(&myMaterial);
	cubeEntity->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(cubeEntity);

	// create the triangle II entity;
	AEntity* triangleEntityB = new AEntity();
	triangleEntityB->setMesh(&triangleMesh);
	triangleEntityB->setMaterial(&myMaterial);
	triangleEntityB->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(triangleEntityB);

}