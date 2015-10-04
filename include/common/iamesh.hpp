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
		virtual class AMaterial* getMaterial() = 0;
		virtual void setMaterial(class AMaterial* a_material) = 0;
		virtual glm::vec3 getCenterPoint() = 0;							// Center point relative to mesh's outer vertices

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
		glm::vec3 centerpoint;


		class AMaterial* material;
		//class IATexture* diffuse;
		//class IATexture* normal;
		//class IATexture* specular;
	};
}