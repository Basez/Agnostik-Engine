#include "asharedh.hpp"
#include "ascenemanager.hpp"
#include "iarender_api.hpp"
#include "aassetmanager.hpp"
#include "aaplication.hpp"
#include "iamesh.hpp"

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
	IAMesh& suzanneMesh = g_application.getAssetManager().loadModel("models/suzanne.obj");
}

void AGN::ASceneManager::addNode()
{

}
