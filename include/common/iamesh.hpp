#pragma once

#include <vector>

namespace AGN
{
	class IAMesh
	{
	public:
		virtual ~IAMesh() {}
		virtual std::string getRelativePath() = 0;
	};

	struct MeshData
	{
		std::vector<uint32_t> indicies;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tagents;
		std::vector<glm::vec3> bitangent;
		std::vector<glm::vec2> textureCoords;

		// TODO:?
		//class BTexture* m_textureDiffuse;
		//class BTexture* m_textureNormal;
		//class BTexture* m_textureSpecular;
	};
}