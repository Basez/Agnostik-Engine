#include "shared.hpp"

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "resource_manager.hpp"
#include "application.hpp"
#include "i_render_api.hpp"
#include "i_device.hpp"
#include "config_manager.hpp"
#include "i_texture.hpp"
#include "i_mesh.hpp"
#include "material.hpp"
#include "i_shader.hpp"
#include "os_utils.hpp"
#include "i_shader_pipeline.hpp"

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace std;
using namespace glm;

AGN::ResourceManager::ResourceManager(class IDevice& a_device)
	: m_device(a_device)
	, m_meshIdCount(0)
	, m_meshCollectionIdCount(0)
	, m_shaderIdCount(0)
	, m_textureIdCount(0)
	, m_materialIdCount(0)
	, m_shaderPipelineIdCount(0)
	, m_defaultMaterial(nullptr)
{

}

AGN::ResourceManager::~ResourceManager()
{
	unloadAll();
}

void AGN::ResourceManager::unloadAll()
{
	while (m_loadedMeshCollections.size() > 0)
	{
		delete m_loadedMeshCollections[0];
		m_loadedMeshCollections.erase(m_loadedMeshCollections.begin());
	}

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

void AGN::ResourceManager::loadDefaults()
{
	// create default material
	std::string defaultMaterialTexturePath = g_configManager.getConfigProperty("default_material");

	m_defaultMaterial = &createMaterial("DefaultMaterial");
	m_defaultMaterial->diffuseColor = vec3(1.0f, 1.0f, 1.0f);
	m_defaultMaterial->specularColor = vec3(0.0f, 0.0f, 0.0f);
	m_defaultMaterial->ambientColor = vec3(0.0f, 0.0f, 0.0f);
	m_defaultMaterial->transparency = 0.0f;
	m_defaultMaterial->diffuseTexture = &loadTexture(defaultMaterialTexturePath.c_str(), ETextureType::TEXTURE_2D);;
	m_defaultMaterial->normalTexture = nullptr;
	m_defaultMaterial->specularTexture = nullptr;
	m_defaultMaterial->specularPower = 100.0f;
}

AGN::MeshCollection& AGN::ResourceManager::loadMeshCollection(std::string a_relativePath, uint32_t additional_assimp_flags, float a_scaleModifier)
{
	// check if it exists
	for (unsigned int i = 0; i < m_loadedMeshCollections.size(); i++)
	{
		if (m_loadedMeshCollections[i]->getRelativePath().compare(a_relativePath) == 0)
		{
			return *m_loadedMeshCollections[i];
		}
	}

	g_log.info("Loading Mesh: %s", a_relativePath.c_str());

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
	std::vector<AGN::Material*> materials;
	materials.reserve(scene->mNumMaterials);

	// start by loading the materials first
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* assimpMaterial = scene->mMaterials[i];
		aiString relativePath = aiString();  // filename
		std::string texturesFolder = g_configManager.getConfigProperty("path_textures");

		aiString materialName;
		assimpMaterial->Get(AI_MATKEY_NAME, materialName);

		AGN::Material& newMaterial = createMaterial(materialName.C_Str());
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
		if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &relativePath) == AI_SUCCESS)
		{
			newMaterial.diffuseTexture = &loadTexture(relativePath.C_Str(), ETextureType::TEXTURE_2D);
		}

		if (assimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &relativePath) == AI_SUCCESS)
		{
			newMaterial.normalTexture = &loadTexture(relativePath.C_Str(), ETextureType::TEXTURE_2D);
		}
		
		if (assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &relativePath) == AI_SUCCESS)
		{
			newMaterial.specularTexture = &loadTexture(relativePath.C_Str(), ETextureType::TEXTURE_2D);
		}

		// color  properties
		aiColor3D diffuseColor(0.f, 0.f, 0.f);
		aiColor3D specularColor(0.f, 0.f, 0.f);
		aiColor3D ambientColor(0.f, 0.f, 0.f);
		float specularPower = 0;

		assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
		assimpMaterial->Get(AI_MATKEY_SHININESS, specularPower);

		newMaterial.diffuseColor = vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
		newMaterial.specularColor = vec3(specularColor.r, specularColor.g, specularColor.b);
		newMaterial.ambientColor = vec3(ambientColor.r, ambientColor.g, ambientColor.b);
		newMaterial.specularPower = specularPower;

		// misc properties
		assimpMaterial->Get(AI_MATKEY_OPACITY, newMaterial.transparency);
	}

	// create objects that defines everything the models consists of
	std::vector<MeshData*> meshDataList;
	meshDataList.reserve(scene->mNumMeshes);

	const aiVector3D zeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		meshDataList.push_back(new MeshData());

		const aiMesh& loadedMesh = *scene->mMeshes[i];

		meshDataList[i]->relativePath = a_relativePath;

		vec3 max(-99999.0f), min(99999.0f);
		for (unsigned int j = 0; j < loadedMesh.mNumVertices; j++)
		{
			const aiVector3D* loadedPos = &(loadedMesh.mVertices[j]);
			const aiVector3D* loadedNormal = &(loadedMesh.mNormals[j]);
			const aiVector3D* loadedTextureCoords = loadedMesh.HasTextureCoords(0) ? &(loadedMesh.mTextureCoords[0][j]) : &zeroVector;
			const aiVector3D* loadedTangents = &(loadedMesh.mTangents[j]);
			const aiVector3D* loadedBitangents = &(loadedMesh.mBitangents[j]);

			meshDataList[i]->positions.push_back(vec3(loadedPos->x * a_scaleModifier, loadedPos->y * a_scaleModifier, loadedPos->z * a_scaleModifier));
			meshDataList[i]->normals.push_back(vec3(loadedNormal->x, loadedNormal->y, loadedNormal->z));
			meshDataList[i]->tangents.push_back(vec3(loadedTangents->x, loadedTangents->y, loadedTangents->z));
			meshDataList[i]->bitangents.push_back(vec3(loadedBitangents->x, loadedBitangents->y, loadedBitangents->z));
			meshDataList[i]->textureCoords.push_back(vec2(loadedTextureCoords->x, loadedTextureCoords->y));

			// calculate max and minimum (for outer most vertices)
			if (loadedPos->x < min.x) min.x = loadedPos->x;
			if (loadedPos->y < min.y) min.y = loadedPos->y;
			if (loadedPos->z < min.z) min.z = loadedPos->z;

			if (loadedPos->x > max.x) max.x = loadedPos->x;
			if (loadedPos->y > max.y) max.y = loadedPos->y;
			if (loadedPos->z > max.z) max.z = loadedPos->z;
			
		}

		meshDataList[i]->centerpoint = min + (max - min) * 0.5f;

		for (unsigned int j = 0; j < loadedMesh.mNumFaces; j++)
		{
			const aiFace& face = loadedMesh.mFaces[j];
			meshDataList[i]->indicies.push_back(face.mIndices[0]);
			meshDataList[i]->indicies.push_back(face.mIndices[1]);
			meshDataList[i]->indicies.push_back(face.mIndices[2]);
		}

		// add loaded materials to meshdata
		meshDataList[i]->material = materials[loadedMesh.mMaterialIndex];
	}

	std::vector<IMesh*> meshList;
	meshList.reserve(scene->mNumMeshes);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		IMesh* newMesh = m_device.createMesh(m_meshIdCount++, meshDataList[i]);
		meshList.push_back(newMesh);
	}
	
	MeshCollection* meshCollection = new MeshCollection(m_meshCollectionIdCount++, a_relativePath, meshList);
	m_loadedMeshCollections.push_back(meshCollection);

	return *meshCollection;
}

AGN::ITexture& AGN::ResourceManager::loadTexture(std::string a_relativePath, ETextureType a_textureType)
{
	// check if it exists
	for (unsigned int i = 0; i < m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i]->getRelativePath().compare(a_relativePath) == 0)
		{
			return dynamic_cast<AGN::ITexture&>(*m_loadedTextures[i]);
		}
	}

	// Load Texture Data
	TextureData* textureData = new TextureData();
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

	ITexture* newTexture = m_device.createTexture(m_textureIdCount++, textureData);

	m_loadedTextures.push_back(newTexture);

	return *newTexture;
}

AGN::IShader& AGN::ResourceManager::createShader(const char* a_shaderSource, EShaderType a_shaderType)
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
	AGN::IShader* newShader = m_device.createShader(m_shaderIdCount++, a_shaderSource, a_shaderType);
	m_shaders.push_back(newShader);

	return *newShader;
}

AGN::IShaderPipeline& AGN::ResourceManager::createShaderPipeline(std::vector<AGN::IShader*> a_shaders)
{
	// TODO: how do we check we have a duplicate shaderpipeline?

	AGN::IShaderPipeline* newShaderPipeline = m_device.createShaderPipeline(m_shaderPipelineIdCount++, a_shaders);
	m_shaderpipelines.push_back(newShaderPipeline);

	return *newShaderPipeline;
}

/** 
	Create a material with the supplied name.
	If a material with that name already exists the existing material gets returned
*/
AGN::Material& AGN::ResourceManager::createMaterial(std::string a_name)
{
	// check if it exists
	for (unsigned int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getName().compare(a_name) == 0)
		{
			g_log.warning("Material with name: %s already exists", a_name.c_str());
			return dynamic_cast<AGN::Material&>(*m_materials[i]);
		}
	}
	
	Material* newMaterial = new Material(m_materialIdCount++, a_name);

	m_materials.push_back(newMaterial);

	return *newMaterial;
}