#include "asharedh.hpp"
#include "aassetmanager.hpp"
#include "amesh_gl.hpp"
#include "asharedh.hpp"
#include "aaplication.hpp"
#include "iarender_api.hpp"
#include "iadevice.hpp"
#include "aconfigmanager.hpp"
#include "afileutils.hpp"

// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
using namespace glm;

AGN::AAssetManager::AAssetManager()
{

}

void AGN::AAssetManager::init()
{
	
}

AGN::IAMesh& AGN::AAssetManager::loadMesh(std::string a_relativePath, uint32_t additional_assimp_flags)
{
	// check if it exists
	for (int i = 0; i < m_meshList.size(); i++)
	{
		if (m_meshList[i]->getRelativePath().compare(a_relativePath) == 0)
		{
			return dynamic_cast<AGN::IAMesh&>(*m_meshList[i]);
		}
	}

	// at this point we know the model doesn't exist yet; load the model
	Assimp::Importer importer;

	// create object that defines everything the model consists of
	MeshData* meshData = new MeshData(); // TODO: delete object after we're done with it (no need to store meshdata on CPU)

	unsigned int flags = additional_assimp_flags | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices; // aiProcess_Triangulate | aiProcess_CalcTangentSpace |

	//if (!a_rightHandCoordinates)
	//{
	//	flags |= aiProcess_FlipWindingOrder;
	//}
	
	// get full path
	string fullPath = g_configManager.getConfigProperty("path_models").append(a_relativePath);

	const aiScene* scene = importer.ReadFile(fullPath.c_str(), flags);

	// If the import failed, report it
	if (!scene)
	{
		g_log.error("Error loading model! %s", importer.GetErrorString());
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

	// load textures/materials into openGL via ShaderManager
	// TODO: Confirm this code works
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		// TODO: TEXTURE LOADING

		/*
		const aiMaterial* material = scene->mMaterials[i];
		int texIndex = 0;
		aiString relativePath;  // filename

		string finalPath = AFileUtils::getPathRelativeToPath(a_relativePath, relativePath.C_Str());

		if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &relativePath) == AI_SUCCESS)
		{
			// TODO: TextureLoading?
			//m_textureDiffuse = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
		}

		if (material->GetTexture(aiTextureType_NORMALS, texIndex, &relativePath) == AI_SUCCESS)
		{
			// TODO: TextureLoading?
			//m_textureNormal = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
		}

		if (material->GetTexture(aiTextureType_SPECULAR, texIndex, &relativePath) == AI_SUCCESS)
		{
			// TODO: TextureLoading?
			//m_textureSpecular = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
		}
		*/
	}


	
	return *g_application.getRenderAPI().getDevice().createMesh(*meshData);

	//
}

/*
void Mesh::loadModel(const char* a_path, unsigned int additional_assimp_flags)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	unsigned int flags = additional_assimp_flags | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices; // aiProcess_Triangulate | aiProcess_CalcTangentSpace |

	//if (!a_rightHandCoordinates)
	//{
	//	flags |= aiProcess_FlipWindingOrder;
	//}

	string fullPath = BPATH(a_path);

	const aiScene* scene = importer.ReadFile(fullPath.c_str(), flags);

	// If the import failed, report it
	if (!scene)
	{
		Log.error("Error loading model! %s", importer.GetErrorString());
		return;
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

			m_positions.push_back(vec3(pPos->x, pPos->y, pPos->z));
			m_normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
			//m_tagents.push_back(vec3(pTangents->x, pTangents->y, pTangents->z));
			//m_bitangent.push_back(vec3(pBitangents->x, pBitangents->y, pBitangents->z));
			m_textureCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
		}

		for (unsigned int j = 0; j < loadedMesh->mNumFaces; j++)
		{
			const aiFace& face = loadedMesh->mFaces[j];
			m_indicies.push_back(face.mIndices[0] + vertexOffset);
			m_indicies.push_back(face.mIndices[1] + vertexOffset);
			m_indicies.push_back(face.mIndices[2] + vertexOffset);
		}
		vertexOffset += loadedMesh->mNumVertices;
	}

	// load textures/materials into openGL via ShaderManager
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* material = scene->mMaterials[i];
		int texIndex = 0;
		aiString relativePath;  // filename

		string finalPath = FileUtils::getPathRelativeToPath(a_path, relativePath.C_Str());

		if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &relativePath) == AI_SUCCESS)
		{
			m_textureDiffuse = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
		}

		if (material->GetTexture(aiTextureType_NORMALS, texIndex, &relativePath) == AI_SUCCESS)
		{
			m_textureNormal = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
		}

		if (material->GetTexture(aiTextureType_SPECULAR, texIndex, &relativePath) == AI_SUCCESS)
		{
			m_textureSpecular = new BTexture(BTextureType::TEXTURE_2D, finalPath.c_str());
		}
	}
}*/