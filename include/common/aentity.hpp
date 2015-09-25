#pragma once

namespace AGN
{
	class AEntity
	{
	public:
		AEntity();

		void setMesh(class IAMesh* a_mesh) { m_mesh = a_mesh; }
		void setMaterial(class AMaterial* a_material) { m_material = a_material; }
		void setPosition(const glm::vec3 a_position) { m_position = a_position; }
		void setRotation(const glm::quat a_rotation) { m_rotation = a_rotation; }
		void setScale(const glm::vec3 a_scale) { m_scale = a_scale; }

		class IAMesh* getMesh() const { return m_mesh; }
		class AMaterial* getMaterial() const { return m_material; }

		glm::vec3 getPosition() const { return m_position; }
		glm::quat getRotation() const { return m_rotation; }
		glm::vec3 getScale() const { return m_scale; }


	private:
		class IAMesh *m_mesh;
		class AMaterial *m_material;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;
	};
}
