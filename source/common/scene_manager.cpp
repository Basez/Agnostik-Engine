#include "shared.hpp"
#include "scene_manager.hpp"
#include "i_render_api.hpp"
#include "resource_manager.hpp"
#include "application.hpp"
#include "i_mesh.hpp"
#include "i_texture.hpp"
#include "material.hpp"
#include "entity.hpp"
#include "camera.hpp"
#include "i_shader.hpp"
#include "i_shader_pipeline.hpp"
#include "camera_controller.hpp"

#include <assimp/postprocess.h>

using namespace glm;

AGN::SceneManager::SceneManager()
{
	
}

AGN::SceneManager::~SceneManager()
{
	
}

void AGN::SceneManager::init()
{
	m_camera = new Camera(vec3(0, 2, 7.5f), quat(vec3(glm::radians(-10.0f),0,0)));
	m_camera->setProjectionRH(60.0f, 0.1f, 10000.0f);

	// create camera controller
	m_cameraController = new CameraController(*m_camera, false);
}

void AGN::SceneManager::loadScene(int16_t a_index)
{
	cleanupScene();

	// TODO: refactor into proper scene management functionality
	switch (a_index)
	{
	case 0:
		loadSponza();
		break;

	case 1:
		loadSibenik();
		break;

	case 2:
		loadSuzannaCrate();
		break;

	case 3:
		loadEmpty();
		break;

	default:
		g_log.error("Invalid scene index!");
		break;
	}
}

void AGN::SceneManager::onWindowUpdated(glm::ivec2 a_dimentions)
{
	m_camera->setProjectionRH(60.0f, 0.1f, 10000.0f);
}

void AGN::SceneManager::update(float a_deltaTime)
{
	m_cameraController->update(a_deltaTime);

	// let skybox entities follow the camera
	// TODO: refactor this into something else?
	for (unsigned int i = 0; i < m_skyboxEntities.size(); i++)
	{
		m_skyboxEntities[i]->setPosition(m_camera->getPosition());
	}
}


void AGN::SceneManager::loadSponza()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	// load meshes
	std::vector<IMesh*> sponzaMeshCollection = resourceManager.loadMeshCollection("sponza/sponza.obj");
	std::vector<IMesh*> skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");

	// create materials
	Material& testMaterial = resourceManager.createMaterial("test_material");
	testMaterial.diffuseTexture = &resourceManager.loadTexture("test.png", ETextureType::TEXTURE_2D);

	Material& crateMaterial = resourceManager.createMaterial("crate_material");
	crateMaterial.diffuseTexture = &resourceManager.loadTexture("market_props_crate_1_texture.png", ETextureType::TEXTURE_2D);

	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D);
	skyboxMaterial.diffuseTexture->setTextureParams((unsigned int)ETextureRenderFlags::USE_CLAMP);
	skyboxMeshCollection[0]->setMaterial(&skyboxMaterial);

	// create entities
	Entity* sponzaEntity = new Entity();
	sponzaEntity->setMeshCollection(sponzaMeshCollection);
	sponzaEntity->setPosition(vec3(0, 0, 0));
	m_entities.push_back(sponzaEntity);

	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	m_skyboxEntities.push_back(skyboxEntity);
}


void AGN::SceneManager::loadSibenik()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	// load meshes
	std::vector<IMesh*> sibenikMeshCollection = resourceManager.loadMeshCollection("sibenik/sibenik.obj");
	std::vector<IMesh*> skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");

	// create materials
	Material& testMaterial = resourceManager.createMaterial("test_material");
	testMaterial.diffuseTexture = &resourceManager.loadTexture("test.png", ETextureType::TEXTURE_2D);

	Material& crateMaterial = resourceManager.createMaterial("crate_material");
	crateMaterial.diffuseTexture = &resourceManager.loadTexture("market_props_crate_1_texture.png", ETextureType::TEXTURE_2D);

	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D);
	skyboxMaterial.diffuseTexture->setTextureParams((unsigned int)ETextureRenderFlags::USE_CLAMP);
	skyboxMeshCollection[0]->setMaterial(&skyboxMaterial);

	// create entities
	Entity* sibenikEntity = new Entity();
	sibenikEntity->setMeshCollection(sibenikMeshCollection);
	sibenikEntity->setPosition(vec3(0, 0, 0));
	m_entities.push_back(sibenikEntity);

	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	m_skyboxEntities.push_back(skyboxEntity);
}

void AGN::SceneManager::loadSuzannaCrate()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	std::vector<IMesh*> crateMeshCollection = resourceManager.loadMeshCollection("crate.dae");
	std::vector<IMesh*> suzanneMeshCollection = resourceManager.loadMeshCollection("suzanne.obj");
	std::vector<IMesh*> skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");
	std::vector<IMesh*> triangleCollection = resourceManager.loadMeshCollection("triangle.obj");

	// create materials
	Material& testMaterial = resourceManager.createMaterial("test_material");
	testMaterial.diffuseTexture = &resourceManager.loadTexture("test.png", ETextureType::TEXTURE_2D);

	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D);

	suzanneMeshCollection[0]->setMaterial(&testMaterial);
	skyboxMeshCollection[0]->setMaterial(&skyboxMaterial);

	// Skybox entity
	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	skyboxEntity->setScale(vec3(10, 10, 10));
	m_skyboxEntities.push_back(skyboxEntity);

	// triangle
	Entity* triangleEntity = new Entity();
	triangleEntity->setMeshCollection(triangleCollection);
	triangleEntity->setPosition(vec3(0, 0, 5));
	m_entities.push_back(triangleEntity);
	
	// create crate entities
	int index = 0;
	for (int x = 0; x < 1; x++)
	{
		for (int y = 0; y < 1; y++)
		{
			for (int z = 0; z < 10; z++)
			{
				// interchange crates & suzannes (worst case scenario in terms of sorting/nosorting)
				if ((++index % 2) == 1)
				{
					// crate
					Entity* crateEntity = new Entity();
					crateEntity->setMeshCollection(crateMeshCollection);
					crateEntity->setPosition(vec3(x * 2, y * 2, z * -2));
					m_entities.push_back(crateEntity);
				}
				else
				{
					// suzanne
					Entity* suzanneEntity = new Entity();
					suzanneEntity->setMeshCollection(suzanneMeshCollection);
					suzanneEntity->setPosition(vec3(x * 2, y * 2, z * -2));
					m_entities.push_back(suzanneEntity);
				}
				
			}
		}
	}
}

void AGN::SceneManager::loadEmpty()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	std::vector<IMesh*> skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");

	// create materials
	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D);

	skyboxMeshCollection[0]->setMaterial(&skyboxMaterial);

	// Skybox entity
	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	skyboxEntity->setScale(vec3(10, 10, 10));
	m_skyboxEntities.push_back(skyboxEntity);
}

void AGN::SceneManager::cleanupScene()
{
	for (Entity* entity : m_entities) delete entity;
	m_entities.clear();

	for (Entity* skyboxEntity : m_skyboxEntities) delete skyboxEntity;
	m_skyboxEntities.clear();

	// TODO: cleanup on GPU size aswell
}
