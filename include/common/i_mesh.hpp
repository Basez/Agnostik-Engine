#pragma once

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
		virtual std::string getName() = 0;
	};

	struct MeshData
	{
		std::string name;	// TODO: Implement
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

	class MeshCollection
	{
	public:
		MeshCollection(uint16_t a_aId, std::string a_relativePath, std::vector<class IMesh*> a_meshlist)
			: m_aId(a_aId)
			, m_relativePath(a_relativePath)
			, m_meshlist(a_meshlist) { }

		~MeshCollection()
		{
			while (m_meshlist.size() > 0)
			{
				delete m_meshlist[0];
				m_meshlist.erase(m_meshlist.begin());
			}
		}

		std::string getRelativePath() { return m_relativePath; };
		uint16_t getAId() { return m_aId; };
		uint16_t getNumMeshes() { return static_cast<uint16_t>(m_meshlist.size()); }
		const std::vector<class IMesh*> getMeshList() { return m_meshlist; }

	private:
		uint16_t m_aId;
		std::string m_relativePath;
		std::vector<class IMesh*> m_meshlist;
	};
}