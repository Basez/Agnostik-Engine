#pragma once

#include <vector>

namespace AGN
{
	class IAMesh
	{
	public:
		virtual ~IAMesh() {}
		virtual std::string getRelativePath() = 0;
		virtual uint16_t getAId() = 0;
	};

	struct AMeshData
	{
		std::string relativePath;
		std::vector<uint32_t> indicies;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tagents;
		std::vector<glm::vec3> bitangent;
		std::vector<glm::vec2> textureCoords;
	};
}