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

#include <assimp/postprocess.h>

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

void AGN::ASceneManager::loadScene()
{
	loadScrambledScene();
	//loadSponza();
}

void AGN::ASceneManager::onWindowUpdated(glm::ivec2 a_dimentions)
{
	m_camera->setProjectionRH(60.0f, 0.1f, 10000.0f);
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

void AGN::ASceneManager::loadScrambledScene()
{
	AResourceManager& resourceManager = g_application.getResourceManager();

	std::vector<IAMesh*> crateMeshCollection = resourceManager.loadMeshCollection("crate.dae");
	std::vector<IAMesh*> suzanneMeshCollection = resourceManager.loadMeshCollection("suzanne.obj");
	std::vector<IAMesh*> skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");
	std::vector<IAMesh*> triangleCollection = resourceManager.loadMeshCollection("triangle.obj");

	// create materials
	AMaterial& testMaterial = resourceManager.createMaterial("test_material");
	testMaterial.diffuseTexture = &resourceManager.loadTexture("test.png", EATextureType::TEXTURE_2D);

	AMaterial& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", EATextureType::TEXTURE_2D);

	suzanneMeshCollection[0]->setMaterial(&testMaterial);
	skyboxMeshCollection[0]->setMaterial(&skyboxMaterial);

	// Skybox entity
	AEntity* skyboxEntity = new AEntity();
	skyboxEntity->setMeshCollection(skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	skyboxEntity->setScale(vec3(10, 10, 10));
	m_skyboxEntities.push_back(skyboxEntity);

	// triangle
	AEntity* triangleEntity = new AEntity();
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
					AEntity* crateEntity = new AEntity();
					crateEntity->setMeshCollection(crateMeshCollection);
					crateEntity->setPosition(vec3(x * 2, y * 2, z * -2));
					m_entities.push_back(crateEntity);
				}
				else
				{
					// suzanne
					AEntity* suzanneEntity = new AEntity();
					suzanneEntity->setMeshCollection(suzanneMeshCollection);
					suzanneEntity->setPosition(vec3(x * 2, y * 2, z * -2));
					m_entities.push_back(suzanneEntity);
				}
				
			}
		}
	}
}

void AGN::ASceneManager::loadSponza()
{
	AResourceManager& resourceManager = g_application.getResourceManager();

	// load meshes
	//std::vector<IAMesh*> sponzaMeshCollection = resourceManager.loadMeshCollection("sponza/sponza.obj");
	std::vector<IAMesh*> sponzaMeshCollection = resourceManager.loadMeshCollection("sibenik/sibenik.obj");
	std::vector<IAMesh*> skyboxMeshCollection = resourceManager.loadMeshCollection("skybox_fixed.obj");

	// create materials
	AMaterial& testMaterial = resourceManager.createMaterial("test_material");
	testMaterial.diffuseTexture = &resourceManager.loadTexture("test.png", EATextureType::TEXTURE_2D);

	AMaterial& crateMaterial = resourceManager.createMaterial("crate_material");
	crateMaterial.diffuseTexture = &resourceManager.loadTexture("market_props_crate_1_texture.png", EATextureType::TEXTURE_2D);

	AMaterial& skyboxMaterial = resourceManager.createMaterial("skybox_material");
	skyboxMaterial.diffuseTexture = &resourceManager.loadTexture("skybox/full2.png", EATextureType::TEXTURE_2D);
	skyboxMaterial.diffuseTexture->setTextureParams((unsigned int)EATextureRenderFlags::USE_CLAMP);
	skyboxMeshCollection[0]->setMaterial(&skyboxMaterial);

	// create entities
	AEntity* sponzaEntity = new AEntity();
	sponzaEntity->setMeshCollection(sponzaMeshCollection);
	sponzaEntity->setPosition(vec3(0, 0, 0));
	m_entities.push_back(sponzaEntity);

	AEntity* skyboxEntity = new AEntity();
	skyboxEntity->setMeshCollection(skyboxMeshCollection);
	skyboxEntity->setPosition(vec3(0, 0, 0));
	m_skyboxEntities.push_back(skyboxEntity);
}
