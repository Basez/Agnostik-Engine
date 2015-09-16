#pragma once

namespace AGN
{
	class AEntity
	{
	public:
		AEntity();

		void setMesh(class IAMesh* a_mesh);
		void setShaderProgram(class IAShaderProgram* a_shaderProgram);

		void setPosition(const glm::vec3 a_position) { m_position = a_position; }
		void setRotation(const glm::quat a_rotation) { m_rotation = a_rotation; }
		void setScale(const glm::vec3 a_scale) { m_scale = a_scale; }

		class IAMesh* getMesh() const { return m_mesh; }
		class IAShaderProgram* getShaderProgram() const { return m_shaderProgram; }

	private:
		class IAMesh *m_mesh;
		class IAShaderProgram *m_shaderProgram;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;
	};
}
