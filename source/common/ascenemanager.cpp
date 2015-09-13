#include "asharedh.hpp"
#include "ascenemanager.hpp"
#include "iarender_api.hpp"
#include "aassetmanager.hpp"
#include "aaplication.hpp"
#include "iamesh.hpp"
#include "iatexture.hpp"

AGN::ASceneManager::ASceneManager()
{
	
}

AGN::ASceneManager::~ASceneManager()
{
	
}

void AGN::ASceneManager::init()
{
	
}

void AGN::ASceneManager::loadTestScene01()
{
	IAMesh& suzanneMesh = g_application.getAssetManager().loadMesh("suzanne.obj");

	IATexture& myTexture = g_application.getAssetManager().loadTexture("test.png", EATextureType::TEXTURE_2D);

}

void AGN::ASceneManager::addNode()
{

}
