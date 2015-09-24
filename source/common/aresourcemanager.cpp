#include "asharedh.hpp"
#include "aresourcemanager.hpp"
#include "asharedh.hpp"
#include "aaplication.hpp"
#include "iarender_api.hpp"
#include "iadevice.hpp"
#include "aconfigmanager.hpp"
#include "afileutils.hpp"
#include "iatexture.hpp"
#include "iamesh.hpp"
#include "amaterial.hpp"
#include "iashader.hpp"

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace std;
using namespace glm;

AGN::AResourceManager::AResourceManager()
	: m_meshIdCount(0)
	, m_shaderIdCount(0)
	, m_textureIdCount(0)
	, m_materialIdCount(0)
	, m_shaderPipelineIdCount(0)
{

}

void AGN::AResourceManager::init()
{
	
}

AGN::IAMesh& AGN::AResourceManager::loadMesh(std::string a_relativePath, uint32_t additional_assimp_flags)
{
	// check if it exists
	for (unsigned int i = 0; i < m_loadedMeshes.size(); i++)
	{
		if (m_loadedMeshes[i]->getRelativePath().compare(a_relativePath) == 0)
		{
			return dynamic_cast<AGN::IAMesh&>(*m_loadedMeshes[i]);
		}
	}

	// at this point we know the model doesn't exist yet; load the model
	// create object that defines everything the model consists of
	AMeshData* meshData = new AMeshData();
	meshData->relativePath = a_relativePath;

	unsigned int flags = additional_assimp_flags | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenNormals;
	
	//if (!a_rightHandCoordinates)
	//{
	//	flags |= aiProcess_FlipWindingOrder;
	//}
	
	// get full path
	string fullPath = g_configManager.getConfigProperty("path_models").append(a_relativePath);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fullPath.c_str(), flags);

	// If the import failed, report it
	if (!scene)
	{
		g_log.error("Error loading model! '%s' the issue: %s", a_relativePath.c_str(), importer.GetErrorString());
	}

	const aiVector3D zeroVector(0.0f, 0.0f, 0.0f);
	unsigned int vertexOffset = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* loadedMesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < loadedMesh->mNumVertices; j++)
		{
			const aiVector3D* pPos = &(loadedMesh->mVertices[j]);
			const aiVector3D* pNormal = &(loadedMesh->mNormals[j]);
			const aiVector3D* pTexCoord = loadedMesh->HasTextureCoords(0) ? &(loadedMesh->mTextureCoords[0][j]) : &zeroVector;
			//const aiVector3D* pTangents = &(loadedMesh->mTangents[j]);
			//const aiVector3D* pBitangents = &(loadedMesh->mBitangents[j]);

			
			meshData->positions.push_back(vec3(pPos->x, pPos->y, pPos->z));
			meshData->normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
			//m_tagents.push_back(vec3(pTangents->x, pTangents->y, pTangents->z));
			//m_bitangent.push_back(vec3(pBitangents->x, pBitangents->y, pBitangents->z));
			meshData->textureCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
		}

		for (unsigned int j = 0; j < loadedMesh->mNumFaces; j++)
		{
			const aiFace& face = loadedMesh->mFaces[j];
			meshData->indicies.push_back(face.mIndices[0] + vertexOffset);
			meshData->indicies.push_back(face.mIndices[1] + vertexOffset);
			meshData->indicies.push_back(face.mIndices[2] + vertexOffset);
		}
		vertexOffset += loadedMesh->mNumVertices;
	}

	/*
	// TODO: load textures/materials into openGL via ShaderManager
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		// TODO: TEXTURE LOADING, add to a queue?
		const aiMaterial* material = scene->mMaterials[i];
		int texIndex = 0;
		aiString relativePath;  // filename
		string finalPath = AFileUtils::getPathRelativeToPath(a_relativePath, relativePath.C_Str());

		if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &relativePath) == AI_SUCCESS)
		{
			// TODO: TextureLoading?
			//m_textureDiffuse = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
			g_log.debug("finalPath.c_str(): %s", finalPath.c_str());
		}

		if (material->GetTexture(aiTextureType_NORMALS, texIndex, &relativePath) == AI_SUCCESS)
		{
			// TODO: TextureLoading?
			//m_textureNormal = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
			g_log.debug("finalPath.c_str(): %s", finalPath.c_str());
		}

		if (material->GetTexture(aiTextureType_SPECULAR, texIndex, &relativePath) == AI_SUCCESS)
		{
			// TODO: TextureLoading?
			//m_textureSpecular = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
			g_log.debug("finalPath.c_str(): %s", finalPath.c_str());
		}
		
	}
	*/
	IAMesh* newMesh = g_application.getRenderAPI().getDevice().createMesh(m_meshIdCount++, meshData);

	m_loadedMeshes.push_back(newMesh);

	return *newMesh;
}

AGN::IATexture& AGN::AResourceManager::loadTexture(std::string a_relativePath, EATextureType a_textureType)
{
	// check if it exists
	for (unsigned int i = 0; i < m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i]->getRelativePath().compare(a_relativePath) == 0)
		{
			return dynamic_cast<AGN::IATexture&>(*m_loadedTextures[i]);
		}
	}

	// Load Texture Data
	ATextureData* textureData = new ATextureData();
	textureData->relativePath = a_relativePath;
	textureData->type = a_textureType;

	// get full path
	string fullPath = g_configManager.getConfigProperty("path_textures").append(a_relativePath);

	unsigned char* loadedData = stbi_load(fullPath.c_str(), &textureData->width, &textureData->height, &textureData->components, 4);

	if (loadedData == nullptr)
	{
		g_log.error("Error loading image: %s", fullPath.c_str());
	}
	
	textureData->buffer = new unsigned int[textureData->width*textureData->height];

	memcpy(textureData->buffer, loadedData, textureData->width*textureData->height*sizeof(uint32_t));

	stbi_image_free(loadedData);

	IATexture* newTexture = g_application.getRenderAPI().getDevice().createTexture(m_textureIdCount++, textureData);

	m_loadedTextures.push_back(newTexture);

	return *newTexture;
}

AGN::IAShader& AGN::AResourceManager::createShader(const char* a_shaderSource, EAShaderType a_shaderType)
{
	// TODO: check if it exists --> doesnt have paths how do we check?
	/*
	for (unsigned int i = 0; i < m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i]->getRelativePath().compare(a_relativePath) == 0)
		{
			return dynamic_cast<AGN::IAShader&>(*m_loadedShaders[i]);
		}
	}
	*/
	AGN::IAShader* newShader = g_application.getRenderAPI().getDevice().createShader(m_shaderIdCount++, a_shaderSource, a_shaderType);
	m_shaders.push_back(newShader);

	return *newShader;
}

AGN::IAShaderPipeline& AGN::AResourceManager::createShaderPipeline(std::vector<AGN::IAShader*> a_shaders)
{
	// TODO: how do we check we have a duplicate shaderpipeline?

	AGN::IAShaderPipeline* newShaderPipeline = g_application.getRenderAPI().getDevice().createShaderPipeline(m_shaderPipelineIdCount++, a_shaders);
	m_shaderpipelines.push_back(newShaderPipeline);

	return *newShaderPipeline;
}

AGN::AMaterial& AGN::AResourceManager::createMaterial(AGN::AMaterialData& a_data)
{
	// check if it exists
	for (unsigned int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getName().compare(a_data.name) == 0)
		{
			return dynamic_cast<AGN::AMaterial&>(*m_materials[i]);
		}
	}
	
	AMaterial* newMaterial = new AMaterial(m_materialIdCount++, a_data);

	m_materials.push_back(newMaterial);

	return *newMaterial;
}