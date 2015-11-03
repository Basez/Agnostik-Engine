#pragma once

#include <vector>

namespace AGN
{
	class IMesh
	{
	public:
		virtual ~IMesh() {}
		virtual std::string getRelativePath() = 0;
		virtual uint16_t getAId() = 0;
		virtual class Material* getMaterial() = 0;
		virtual void setMaterial(class Material* a_material) = 0;
		virtual glm::vec3 getCenterPoint() = 0;							// Center point relative to mesh's outer vertices
		virtual uint32_t getIndexCount() const = 0;

	};

	struct MeshData
	{
		std::string relativePath;
		std::vector<uint32_t> indicies;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		std::vector<glm::vec2> textureCoords;
		glm::vec3 centerpoint;


		class Material* material;
	};
}