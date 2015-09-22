#include "asharedh.hpp"
#include "ascenemanager.hpp"
#include "iarender_api.hpp"
#include "aresourcemanager.hpp"
#include "aaplication.hpp"
#include "iamesh.hpp"
#include "iatexture.hpp"
#include "amaterial.hpp"
#include "aentity.hpp"
#include "acamera.hpp"
#include "iashader.hpp"
#include "iashaderpipeline.hpp"

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
	m_camera->setPosition(glm::vec3(0, 0, 10));
	m_camera->setProjectionRH(60.0f, 0.1f, 10000.0f);
}

void AGN::ASceneManager::loadTestScene01()
{
	AResourceManager& resourceManager = g_application.getResourceManager();

	IAMesh& suzanneMesh = resourceManager.loadMesh("suzanne.obj");
	IAMesh& triangleMesh = resourceManager.loadMesh("triangle.obj");
	IAMesh& cubeMesh = resourceManager.loadMesh("cube.obj");
	IAMesh& skyboxMesh = resourceManager.loadMesh("skybox.obj");		

	// TODO: bind textures to shader?
	IATexture& myTexture = resourceManager.loadTexture("test.png", EATextureType::TEXTURE_2D);

	AMaterialData materialData;
	materialData.name = "meshmaterial";
	materialData.diffuseTexture = &myTexture;
	AMaterial& myMaterial = resourceManager.createMaterial(materialData);

	// load shaders
	std::vector<AGN::IAShader*> meshShaders;
	meshShaders.push_back(&resourceManager.createShader(g_shader_mesh_vert, EAShaderType::VertexShader));
	meshShaders.push_back(&resourceManager.createShader(g_shader_mesh_pix, EAShaderType::PixelShader));
	IAShaderPipeline& meshShaderPipeline = resourceManager.createShaderPipeline(meshShaders);

	// create the triangle I entity;
	AEntity* triangleEntityA = new AEntity();
	triangleEntityA->setMesh(&triangleMesh);
	triangleEntityA->setMaterial(&myMaterial);
	triangleEntityA->setShaderPipeline(&meshShaderPipeline);
	triangleEntityA->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(triangleEntityA);

	// create the suzanna entity;
	AEntity* suzanneEntity = new AEntity();
	suzanneEntity->setMesh(&suzanneMesh);
	suzanneEntity->setMaterial(&myMaterial);
	suzanneEntity->setShaderPipeline(&meshShaderPipeline);
	suzanneEntity->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(suzanneEntity);

	// create the cube entity;
	AEntity* cubeEntity = new AEntity();
	cubeEntity->setMesh(&cubeMesh);
	cubeEntity->setMaterial(&myMaterial);
	cubeEntity->setShaderPipeline(&meshShaderPipeline);
	cubeEntity->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(cubeEntity);

	// create the triangle II entity;
	AEntity* triangleEntityB = new AEntity();
	triangleEntityB->setMesh(&triangleMesh);
	triangleEntityB->setMaterial(&myMaterial);
	triangleEntityB->setShaderPipeline(&meshShaderPipeline);
	triangleEntityB->setPosition(glm::vec3(0, 0, -10));
	m_entities.push_back(triangleEntityB);
}