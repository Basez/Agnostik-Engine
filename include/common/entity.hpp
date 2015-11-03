#pragma once

#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace AGN
{
	class Entity
	{
	public:
		Entity()
			: m_position(glm::vec3())
			, m_rotation(glm::quat())
			, m_scale(glm::vec3(1)) {}

		void setMeshCollection(class std::vector<class IMesh*> a_meshes) { m_meshCollection = a_meshes; }
		void setPosition(const glm::vec3 a_position) { m_position = a_position; }
		void setRotation(const glm::quat a_rotation) { m_rotation = a_rotation; }
		void setScale(const glm::vec3 a_scale) { m_scale = a_scale; }

		class std::vector<class IMesh*> getMeshCollection() const { return m_meshCollection; }

		glm::vec3 getPosition() const { return m_position; }
		glm::quat getRotation() const { return m_rotation; }
		glm::vec3 getScale() const { return m_scale; }


	private:
		class std::vector<class IMesh*> m_meshCollection;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;
	};
}

