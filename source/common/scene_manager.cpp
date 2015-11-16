#include "shared.hpp"
#include <assimp/postprocess.h>
#include "camera_controller.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

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

using namespace glm;

AGN::SceneManager::SceneManager()
	: m_camera(nullptr)
	, m_cameraController(nullptr)
	, m_currentSceneIndex(-1)
{
	
}

AGN::SceneManager::~SceneManager()
{
	unloadScene();

	delete m_cameraController;
	delete m_camera;
}

void AGN::SceneManager::init()
{
	m_camera = new Camera(vec3(), quat());
	m_camera->setProjectionRH(60.0f, 0.1f, 10000.0f);

	// create camera controller
	m_cameraController = new CameraController(*m_camera, false);
}

void AGN::SceneManager::loadScene(int16_t a_index)
{
	// ensure nothing else is in scene
	unloadScene();

	// TODO: refactor into proper scene management functionality
	switch (a_index)
	{
	case 0:
		loadCrytekSponza();
		break;

	case 1:
		loadSponza();
		break;

	case 2:
		loadSibenik();
		break;

	case 3:
		loadSuzannaCrate();
		break;

	case 4:
		loadEmpty();
		break;

	default:
		g_log.error("Invalid scene index!");
		break;
	}

	m_currentSceneIndex = a_index;
}

void AGN::SceneManager::onWindowUpdated(glm::ivec2 a_dimensions)
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


void AGN::SceneManager::loadCrytekSponza()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	m_cameraController->reset(vec3(9.0f, 7.96f, -0.60f), vec2(-15.0f, 90.0f));

	// load meshes
	MeshCollection& sponzaMeshCollection = resourceManager.loadMeshCollection("sponza_crytek/sponza.obj", 0 , 0.012f);
	MeshCollection& skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");

	// create materials
	Material& testMaterial = resourceManager.createMaterial("test_material");
	testMaterial.diffuseTexture = &resourceManager.loadTexture("test.png", ETextureType::TEXTURE_2D);

	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D, (unsigned int)ETextureRenderFlags::USE_CLAMP);
	skyboxMeshCollection.getMeshList()[0]->setMaterial(&skyboxMaterial); // TODO: change to getByName!

	Entity* sponzaEntity = new Entity();
	sponzaEntity->setMeshCollection(&sponzaMeshCollection);
	sponzaEntity->setPosition(vec3(0, 0, 0));
	m_entities.push_back(sponzaEntity);

	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(&skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	m_skyboxEntities.push_back(skyboxEntity);
}

void AGN::SceneManager::loadSponza()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	m_cameraController->reset(vec3(-8.6f, 7.7f, 0.0f), vec2(-15.0f, -90.0f));

	// load meshes
	MeshCollection& sponzaMeshCollection = resourceManager.loadMeshCollection("sponza/sponza.obj");
	MeshCollection& skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");

	// create materials
	Material& testMaterial = resourceManager.createMaterial("test_material");
	testMaterial.diffuseTexture = &resourceManager.loadTexture("test.png", ETextureType::TEXTURE_2D);

	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D, (unsigned int)ETextureRenderFlags::USE_CLAMP);
	skyboxMeshCollection.getMeshList()[0]->setMaterial(&skyboxMaterial); // TODO: change to getByName!

	// create entities
	Entity* sponzaEntity = new Entity();
	sponzaEntity->setMeshCollection(&sponzaMeshCollection);
	sponzaEntity->setPosition(vec3(0, 0, 0));
	m_entities.push_back(sponzaEntity);

	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(&skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	m_skyboxEntities.push_back(skyboxEntity);
}

void AGN::SceneManager::loadSibenik()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	m_cameraController->reset(vec3(-17.35f, 1.55f, -0.09f), vec2(-15.0f, -90.0f));

	// load meshes
	MeshCollection& sibenikMeshCollection = resourceManager.loadMeshCollection("sibenik/sibenik.obj");
	MeshCollection& skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");

	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D, (unsigned int)ETextureRenderFlags::USE_CLAMP);
	skyboxMeshCollection.getMeshList()[0]->setMaterial(&skyboxMaterial); // TODO: change to getByName!

	// create entities
	Entity* sibenikEntity = new Entity();
	sibenikEntity->setMeshCollection(&sibenikMeshCollection);
	sibenikEntity->setPosition(vec3(0, 0, 0));
	m_entities.push_back(sibenikEntity);

	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(&skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	m_skyboxEntities.push_back(skyboxEntity);
}

void AGN::SceneManager::loadSuzannaCrate()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	m_cameraController->reset(vec3(13.0f, 10.0f, 22.0f), vec2(-15.0f, 0.0f));

	MeshCollection& crateMeshCollection = resourceManager.loadMeshCollection("crate.dae");
	MeshCollection& suzanneMeshCollection = resourceManager.loadMeshCollection("suzanne_hi_res.obj");
	MeshCollection& skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");
	MeshCollection& triangleCollection = resourceManager.loadMeshCollection("triangle.obj");

	// create materials
	Material& testMaterial = resourceManager.createMaterial("test_material");
	testMaterial.diffuseTexture = &resourceManager.loadTexture("test.png", ETextureType::TEXTURE_2D);
	testMaterial.specularPower = 200.0f;

	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D, (unsigned int)ETextureRenderFlags::USE_CLAMP);

	suzanneMeshCollection.getMeshList()[0]->setMaterial(&testMaterial); // TODO: change to getByName!
	skyboxMeshCollection.getMeshList()[0]->setMaterial(&skyboxMaterial); // TODO: change to getByName!

	// Skybox entity
	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(&skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	skyboxEntity->setScale(vec3(10, 10, 10));
	m_skyboxEntities.push_back(skyboxEntity);

	// triangle
	Entity* triangleEntity = new Entity();
	triangleEntity->setMeshCollection(&triangleCollection);
	triangleEntity->setPosition(vec3(0, 0, 5));
	m_entities.push_back(triangleEntity);
	
	// crate
	//Entity* crateEntity = new Entity();
	//crateEntity->setMeshCollection(&crateMeshCollection);
	//crateEntity->setPosition(vec3(0));
	//crateEntity->setRotation(quat(glm::rotate(45.0f, 1.0f, 0.0f, 0.0f)));
	//m_entities.push_back(crateEntity);
	//
	//// suzanne
	//Entity* suzanneEntity = new Entity();
	//suzanneEntity->setMeshCollection(&suzanneMeshCollection);
	//suzanneEntity->setPosition(vec3(0, 0, 0));
	//m_entities.push_back(suzanneEntity);

	
	// create crate entities
	int index = 0;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 1; y++)
		{
			for (int z = 0; z < 100; z++)
			{
				// interchange crates & suzannes (worst case scenario in terms of sorting/nosorting)
				if ((++index % 2) == 1)
				{
					// crate
					Entity* crateEntity = new Entity();
					crateEntity->setMeshCollection(&crateMeshCollection);
					crateEntity->setPosition(vec3(x * 3, y * 3, z * -3));
					m_entities.push_back(crateEntity);
				}
				else
				{
					// suzanne
					Entity* suzanneEntity = new Entity();
					suzanneEntity->setMeshCollection(&suzanneMeshCollection);
					suzanneEntity->setPosition(vec3(x * 3, y * 3, z * -3));
					m_entities.push_back(suzanneEntity);
				}
			}
		}
	}
}

void AGN::SceneManager::loadEmpty()
{
	ResourceManager& resourceManager = g_application.getResourceManager();

	MeshCollection& skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");

	// create materials
	Material& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", ETextureType::TEXTURE_2D);

	skyboxMeshCollection.getMeshList()[0]->setMaterial(&skyboxMaterial); // TODO: change to getByName!

	// Skybox entity
	Entity* skyboxEntity = new Entity();
	skyboxEntity->setMeshCollection(&skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	skyboxEntity->setScale(vec3(10, 10, 10));
	m_skyboxEntities.push_back(skyboxEntity);
}

void AGN::SceneManager::unloadScene()
{
	for (Entity* entity : m_entities) delete entity;
	m_entities.clear();

	for (Entity* skyboxEntity : m_skyboxEntities) delete skyboxEntity;
	m_skyboxEntities.clear();
}
