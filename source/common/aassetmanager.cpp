#include "asharedh.hpp"
#include "aassetmanager.hpp"
#include "amesh_gl.hpp"
#include "asharedh.hpp"

// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


AGN::AAssetManager::AAssetManager()
{

}

void AGN::AAssetManager::init()
{
	
}

AGN::IAMesh& AGN::AAssetManager::loadModel(std::string a_relativePath)
{
	// check if it exists
	for (int i = 0; i < m_meshList.size(); i++)
	{
		if (m_meshList[i]->getRelativePath().compare(a_relativePath) == 0)
		{
			return dynamic_cast<AGN::IAMesh&>(*m_meshList[i]);
		}
	}

	// TODO: doesnt exist yet, load the model
	//g_application.getRenderAPI().getDevice().
	
}
