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
#include "acameracontroller.hpp"

// shaders
#include "shader_mesh_pix.hpp"
#include "shader_mesh_vert.hpp"

using namespace glm;

AGN::ASceneManager::ASceneManager()
{
	
}

AGN::ASceneManager::~ASceneManager()
{
	
}

void AGN::ASceneManager::init()
{
	m_camera = new ACamera(vec3(0, 2, 7.5f), quat(vec3(glm::radians(-10.0f),0,0)));
	m_camera->setProjectionRH(60.0f, 0.1f, 10000.0f);

	// create camera controller
	m_cameraController = new ACameraController(*m_camera, false);
}

void AGN::ASceneManager::update(float a_deltaTime)
{
	m_cameraController->update(a_deltaTime);

	// let skybox entities follow the camera
	// TODO: refactor this into something else?
	for (unsigned int i = 0; i < m_skyboxEntities.size(); i++)
	{
		m_skyboxEntities[i]->setPosition(m_camera->getPosition());
	}
}

void AGN::ASceneManager::loadTestScene01()
{
	AResourceManager& resourceManager = g_application.getResourceManager();

	IAMesh& sibenixMesh = resourceManager.loadMesh("sibenik/sibenik.obj");
	IAMesh& crateMesh = resourceManager.loadMesh("crate_001.dae");
	IAMesh& suzanneMesh = resourceManager.loadMesh("suzanne.obj");
	IAMesh& triangleMesh = resourceManager.loadMesh("triangle.obj");
	IAMesh& cubeMesh = resourceManager.loadMesh("cube.obj");
	IAMesh& skyboxMesh = resourceManager.loadMesh("skybox.obj");

	// create materials
	AMaterialData testMatData;
	testMatData.name = "testMaterial";
	testMatData.diffuseTexture = &resourceManager.loadTexture("test.png", EATextureType::TEXTURE_2D);
	AMaterial& testMaterial = resourceManager.createMaterial(testMatData);

	AMaterialData crateMatData;
	crateMatData.name = "crateMaterial";
	crateMatData.diffuseTexture = &resourceManager.loadTexture("market_props_crate_1_texture.png", EATextureType::TEXTURE_2D);
	AMaterial& crateMaterial = resourceManager.createMaterial(crateMatData);

	// create shaders
	std::vector<AGN::IAShader*> meshShaders;
	meshShaders.push_back(&resourceManager.createShader(g_shader_mesh_vert, EAShaderType::VertexShader));
	meshShaders.push_back(&resourceManager.createShader(g_shader_mesh_pix, EAShaderType::PixelShader));
	IAShaderPipeline& meshShaderPipeline = resourceManager.createShaderPipeline(meshShaders);

	// create the triangle I entity;
	AEntity* triangleEntityA = new AEntity();
	triangleEntityA->setMesh(&triangleMesh);
	triangleEntityA->setMaterial(&testMaterial);
	triangleEntityA->setShaderPipeline(&meshShaderPipeline);
	triangleEntityA->setPosition(vec3(-5, 0, 0));
	m_entities.push_back(triangleEntityA);

	// create the suzanna entity;
	AEntity* suzanneEntity = new AEntity();
	suzanneEntity->setMesh(&suzanneMesh);
	suzanneEntity->setMaterial(&testMaterial);
	suzanneEntity->setShaderPipeline(&meshShaderPipeline);
	suzanneEntity->setPosition(vec3(0, 0, 0));
	m_entities.push_back(suzanneEntity);

	// create the cube entity;
	AEntity* cubeEntity = new AEntity();
	cubeEntity->setMesh(&cubeMesh);
	cubeEntity->setMaterial(&testMaterial);
	cubeEntity->setShaderPipeline(&meshShaderPipeline);
	cubeEntity->setPosition(vec3(5, 0, 0));
	m_entities.push_back(cubeEntity);

	// create the triangle II entity;
	AEntity* triangleEntityB = new AEntity();
	triangleEntityB->setMesh(&triangleMesh);
	triangleEntityB->setMaterial(&testMaterial);
	triangleEntityB->setShaderPipeline(&meshShaderPipeline);
	triangleEntityB->setPosition(vec3(5, 0, 0));
	m_entities.push_back(triangleEntityB);

	// create crate entities
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			for (int z = 0; z < 10; z++)
			{
				AEntity* crateEntity = new AEntity();
				crateEntity->setMesh(&crateMesh);
				crateEntity->setMaterial(&crateMaterial);
				crateEntity->setShaderPipeline(&meshShaderPipeline);
				crateEntity->setPosition(vec3(x*2, y*2, z*2));
				m_entities.push_back(crateEntity);
			}
		}
	}


}

void AGN::ASceneManager::loadScrambledScene()
{
	AResourceManager& resourceManager = g_application.getResourceManager();

	IAMesh& crateMesh = resourceManager.loadMesh("crate_001.dae");
	IAMesh& suzanneMesh = resourceManager.loadMesh("suzanne.obj");
	IAMesh& skyboxMesh = resourceManager.loadMesh("skybox.obj");

	// create materials
	AMaterialData testMatData;
	testMatData.name = "test_material";
	testMatData.diffuseTexture = &resourceManager.loadTexture("test.png", EATextureType::TEXTURE_2D);
	AMaterial& testMaterial = resourceManager.createMaterial(testMatData);

	AMaterialData crateMatData;
	crateMatData.name = "crate_material";
	crateMatData.diffuseTexture = &resourceManager.loadTexture("market_props_crate_1_texture.png", EATextureType::TEXTURE_2D);
	AMaterial& crateMaterial = resourceManager.createMaterial(crateMatData);

	AMaterialData skyboxMatData;
	skyboxMatData.name = "skybox_material";
	skyboxMatData.diffuseTexture = &resourceManager.loadTexture("skybox_texture.jpg", EATextureType::TEXTURE_2D);
	AMaterial& skyboxMaterial = resourceManager.createMaterial(skyboxMatData);

	// create shaders
	std::vector<AGN::IAShader*> meshShaders;
	meshShaders.push_back(&resourceManager.createShader(g_shader_mesh_vert, EAShaderType::VertexShader));
	meshShaders.push_back(&resourceManager.createShader(g_shader_mesh_pix, EAShaderType::PixelShader));
	IAShaderPipeline& meshShaderPipeline = resourceManager.createShaderPipeline(meshShaders);

	// Skybox entity
	AEntity* skyboxEntity = new AEntity();
	skyboxEntity->setMesh(&skyboxMesh);
	skyboxEntity->setMaterial(&skyboxMaterial);
	skyboxEntity->setShaderPipeline(&meshShaderPipeline);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	m_skyboxEntities.push_back(skyboxEntity);

	// create crate entities
	int index = 0;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			for (int z = 0; z < 10; z++)
			{
				// interchange crates & suzannes (worst case scenario in terms of sorting/nosorting)
				if ((++index % 2) == 1)
				{
					// crate
					AEntity* crateEntity = new AEntity();
					crateEntity->setMesh(&crateMesh);
					crateEntity->setMaterial(&crateMaterial);
					crateEntity->setShaderPipeline(&meshShaderPipeline);
					crateEntity->setPosition(vec3(x * 2, y * 2, z * 2));
					m_entities.push_back(crateEntity);
				}
				else
				{
					// suzanne
					AEntity* suzanneEntity = new AEntity();
					suzanneEntity->setMesh(&suzanneMesh);
					suzanneEntity->setMaterial(&testMaterial);
					suzanneEntity->setShaderPipeline(&meshShaderPipeline);
					suzanneEntity->setPosition(vec3(x * 2, y * 2, z * 2));
					m_entities.push_back(suzanneEntity);
				}
				
			}
		}
	}
}
