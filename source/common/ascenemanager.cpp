#include "asharedh.hpp"
#include "ascenemanager.hpp"
#include "iarender_api.hpp"
#include "aresourcemanager.hpp"
#include "aaplication.hpp"
#include "iamesh.hpp"
#include "iatexture.hpp"
#include "iashaderprogram.hpp"

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
	
}

void AGN::ASceneManager::loadTestScene01()
{
	IAMesh& suzanneMesh = g_application.getResourceManager().loadMesh("suzanne.obj");

	IATexture& myTexture = g_application.getResourceManager().loadTexture("test.png", EATextureType::TEXTURE_2D);

	AShaderProgramData shaderData;
	shaderData.vertexShader = g_shader_mesh_vert;
	shaderData.pixelShader = g_shader_mesh_pix;
	IAShaderProgram& myShaderProgram = g_application.getResourceManager().loadShaderProgram(shaderData);
}

void AGN::ASceneManager::addNode()
{

}
