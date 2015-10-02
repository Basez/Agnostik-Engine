#pragma once

namespace AGN
{
	class AEntity
	{
	public:
		AEntity()
			: m_material(nullptr)
			, m_position(glm::vec3())
			, m_rotation(glm::quat())
			, m_scale(glm::vec3(1)) {}

		void setMeshCollection(class std::vector<class IAMesh*> a_meshes) { m_meshCollection = a_meshes; }
		void setMaterial(class AMaterial* a_material) { m_material = a_material; }
		void setPosition(const glm::vec3 a_position) { m_position = a_position; }
		void setRotation(const glm::quat a_rotation) { m_rotation = a_rotation; }
		void setScale(const glm::vec3 a_scale) { m_scale = a_scale; }

		class std::vector<class IAMesh*> getMeshCollection() const { return m_meshCollection; }

		class AMaterial* getMaterial() const { return m_material; }

		glm::vec3 getPosition() const { return m_position; }
		glm::quat getRotation() const { return m_rotation; }
		glm::vec3 getScale() const { return m_scale; }


	private:
		class std::vector<class IAMesh*> m_meshCollection;
		class AMaterial *m_material;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;
	};
}

