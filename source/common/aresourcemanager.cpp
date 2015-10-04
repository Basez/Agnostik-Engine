#include "asharedh.hpp"
#include "aresourcemanager.hpp"
#include "aaplication.hpp"
#include "iarender_api.hpp"
#include "iadevice.hpp"
#include "aconfigmanager.hpp"
#include "iatexture.hpp"
#include "iamesh.hpp"
#include "amaterial.hpp"
#include "iashader.hpp"
#include "aosutils.hpp"
#include "iashaderpipeline.hpp"

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace std;
using namespace glm;

AGN::AResourceManager::AResourceManager(class IADevice& a_device)
	: m_device(a_device)
	, m_meshIdCount(0)
	, m_shaderIdCount(0)
	, m_textureIdCount(0)
	, m_materialIdCount(0)
	, m_shaderPipelineIdCount(0)
	, m_defaultMaterial(nullptr)
{

}

AGN::AResourceManager::~AResourceManager()
{
	// TODO: Delete Meshes
	while (m_materials.size() > 0)
	{
		delete m_materials[0];
		m_materials.erase(m_materials.begin());
	}
	
	while (m_loadedTextures.size() > 0)
	{
		delete m_loadedTextures[0];
		m_loadedTextures.erase(m_loadedTextures.begin());
	}
	
	while (m_shaderpipelines.size() > 0)
	{
		delete m_shaderpipelines[0];
		m_shaderpipelines.erase(m_shaderpipelines.begin());
	}

	while (m_shaders.size() > 0)
	{
		delete m_shaders[0];
		m_shaders.erase(m_shaders.begin());
	}


}

void AGN::AResourceManager::loadDefaults()
{
	// create default material
	std::string defaultMaterialTexturePath = g_configManager.getConfigProperty("default_material");

	m_defaultMaterial = &createMaterial("DefaultMaterial");
	m_defaultMaterial->diffuseColor = vec3(1.0f, 1.0f, 1.0f);
	m_defaultMaterial->specularColor = vec3(0.0f, 0.0f, 0.0f);
	m_defaultMaterial->ambientColor = vec3(0.0f, 0.0f, 0.0f);
	m_defaultMaterial->transparency = 0.0f;
	m_defaultMaterial->diffuseTexture = &loadTexture(defaultMaterialTexturePath.c_str(), EATextureType::TEXTURE_2D);;
	m_defaultMaterial->normalTexture = nullptr;
	m_defaultMaterial->specularTexture = nullptr;
}

class std::vector<AGN::IAMesh*> AGN::AResourceManager::loadMeshCollection(std::string a_relativePath, uint32_t additional_assimp_flags)
{
	unsigned int flags = additional_assimp_flags;
	flags |= aiProcess_SortByPType;
	flags |= aiProcess_JoinIdenticalVertices;	// Joins identical vertex data sets within all meshes
	flags |= aiProcess_Triangulate;				// triangulate the mesh
	flags |= aiProcess_CalcTangentSpace;		// calculate tangents & bitangents
	flags |= aiProcess_GenNormals;				// calculate normals
	flags |= aiProcess_OptimizeMeshes;			// reduce the number of meshes, thus number of drawcalls
	flags |= aiProcess_OptimizeGraph;			// optimize the scene hierarchy, combine groups etc
	flags |= aiProcess_FlipUVs;					// Flip UV's // TODO: dependent on GFX API?

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

	// create objects that defines everything the meshes consists of
	std::vector<AGN::AMaterial*> materials;
	materials.reserve(scene->mNumMaterials);

	// start by loading the materials first
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* assimpMaterial = scene->mMaterials[i];
		aiString relativePath;  // filename
		std::string texturesFolder = g_configManager.getConfigProperty("path_textures");

		aiString materialName;
		assimpMaterial->Get(AI_MATKEY_NAME, materialName);

		AGN::AMaterial& newMaterial = createMaterial(materialName.C_Str());
		materials.push_back(&newMaterial);

		// skip default materials (assigned by assimp whenever an obj doesnt have a material attached to an obj)
		if (strcmp(materialName.C_Str(), "DefaultMaterial") == 0) continue;

		int diffuseCount = assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE);
		int normalCount = assimpMaterial->GetTextureCount(aiTextureType_NORMALS);
		int specularCount = assimpMaterial->GetTextureCount(aiTextureType_SPECULAR);

		if (diffuseCount > 1 || normalCount > 1 || specularCount > 1)
		{
			g_log.warning("more than singular texture detected?");
		}

		if (diffuseCount < 1)
		{
			g_log.warning("Material without diffuse texture detected! %s", materialName.C_Str());
		}

		// load textures
		if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, NULL, &relativePath) == AI_SUCCESS)
		{
			newMaterial.diffuseTexture = &loadTexture(relativePath.C_Str(), EATextureType::TEXTURE_2D);
		}

		if (assimpMaterial->GetTexture(aiTextureType_NORMALS, NULL, &relativePath) == AI_SUCCESS)
		{
			newMaterial.normalTexture = &loadTexture(relativePath.C_Str(), EATextureType::TEXTURE_2D);
		}

		if (assimpMaterial->GetTexture(aiTextureType_SPECULAR, NULL, &relativePath) == AI_SUCCESS)
		{
			newMaterial.specularTexture = &loadTexture(relativePath.C_Str(), EATextureType::TEXTURE_2D);
		}

		// color  properties
		aiColor3D diffuseColor(0.f, 0.f, 0.f);
		aiColor3D specularColor(0.f, 0.f, 0.f);
		aiColor3D ambientColor(0.f, 0.f, 0.f);
		assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
		newMaterial.diffuseColor = vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
		newMaterial.specularColor = vec3(specularColor.r, specularColor.g, specularColor.b);
		newMaterial.ambientColor = vec3(ambientColor.r, ambientColor.g, ambientColor.b);

		// misc properties
		assimpMaterial->Get(AI_MATKEY_OPACITY, newMaterial.transparency);
	}

	// create objects that defines everything the models consists of
	AMeshData* meshData = new AMeshData[scene->mNumMeshes];

	const aiVector3D zeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh& loadedMesh = *scene->mMeshes[i];
		AMeshData& newMeshData = meshData[i];
		newMeshData.relativePath = a_relativePath;

		vec3 max(-99999.0f), min(99999.0f);
		for (unsigned int j = 0; j < loadedMesh.mNumVertices; j++)
		{
			const aiVector3D* pPos = &(loadedMesh.mVertices[j]);
			const aiVector3D* pNormal = &(loadedMesh.mNormals[j]);
			const aiVector3D* pTexCoord = loadedMesh.HasTextureCoords(0) ? &(loadedMesh.mTextureCoords[0][j]) : &zeroVector;
			//const aiVector3D* pTangents = &(loadedMesh.mTangents[j]);
			//const aiVector3D* pBitangents = &(loadedMesh.mBitangents[j]);

			newMeshData.positions.push_back(vec3(pPos->x, pPos->y, pPos->z));
			newMeshData.normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
			//newMeshData.tangents.push_back(vec3(pTangents->x, pTangents->y, pTangents->z));
			//newMeshData.bitangents.push_back(vec3(pBitangents->x, pBitangents->y, pBitangents->z));
			newMeshData.textureCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));

			// calculate max and minimum (for outer most vertices)
			if (pPos->x < min.x) min.x = pPos->x;
			if (pPos->y < min.y) min.y = pPos->y;
			if (pPos->z < min.z) min.z = pPos->z;

			if (pPos->x > max.x) max.x = pPos->x;
			if (pPos->y > max.y) max.y = pPos->y;
			if (pPos->z > max.z) max.z = pPos->z;
		}

		newMeshData.centerpoint = min + (max - min) * 0.5f;

		for (unsigned int j = 0; j < loadedMesh.mNumFaces; j++)
		{
			const aiFace& face = loadedMesh.mFaces[j];
			newMeshData.indicies.push_back(face.mIndices[0]);
			newMeshData.indicies.push_back(face.mIndices[1]);
			newMeshData.indicies.push_back(face.mIndices[2]);
		}

		// add loaded materials to meshdata
		newMeshData.material = materials[loadedMesh.mMaterialIndex];
	}

	std::vector<AGN::IAMesh*> meshCollection;
	meshCollection.reserve(scene->mNumMeshes);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		IAMesh* newMesh = m_device.createMesh(m_meshIdCount++, &meshData[i]);
		meshCollection.push_back(newMesh);
	}
	
	return meshCollection;
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

	IATexture* newTexture = m_device.createTexture(m_textureIdCount++, textureData);

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
	AGN::IAShader* newShader = m_device.createShader(m_shaderIdCount++, a_shaderSource, a_shaderType);
	m_shaders.push_back(newShader);

	return *newShader;
}

AGN::IAShaderPipeline& AGN::AResourceManager::createShaderPipeline(std::vector<AGN::IAShader*> a_shaders)
{
	// TODO: how do we check we have a duplicate shaderpipeline?

	AGN::IAShaderPipeline* newShaderPipeline = m_device.createShaderPipeline(m_shaderPipelineIdCount++, a_shaders);
	m_shaderpipelines.push_back(newShaderPipeline);

	return *newShaderPipeline;
}

/** 
	Create a material with the supplied name.
	If a material with that name already exists the existing material gets returned
*/
AGN::AMaterial& AGN::AResourceManager::createMaterial(std::string a_name)
{
	// check if it exists
	for (unsigned int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getName().compare(a_name) == 0)
		{
			g_log.warning("Material with name: %s already exists", a_name.c_str());
			return dynamic_cast<AGN::AMaterial&>(*m_materials[i]);
		}
	}
	
	AMaterial* newMaterial = new AMaterial(m_materialIdCount++, a_name);

	m_materials.push_back(newMaterial);

	return *newMaterial;
}