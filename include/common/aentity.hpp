#pragma once

namespace AGN
{
	class AEntity
	{
	public:
		AEntity();

		void setMesh(class IAMesh* a_mesh);
		void setMaterial(class AMaterial* a_material);
		void setShaderPipeline(class IAShaderPipeline* a_shaderPipeline);
		void setPosition(const glm::vec3 a_position) { m_position = a_position; }
		void setRotation(const glm::quat a_rotation) { m_rotation = a_rotation; }
		void setScale(const glm::vec3 a_scale) { m_scale = a_scale; }

		class IAMesh* getMesh() const { return m_mesh; }
		class AMaterial* getMaterial() const { return m_material; }
		class IAShaderPipeline* getShaderPipeline() const { return m_shaderPipeline; }

		glm::vec3 getPosition() const { return m_position; }
		glm::quat getRotation() const { return m_rotation; }
		glm::vec3 getScale() const { return m_scale; }


	private:
		class IAMesh *m_mesh;
		class AMaterial *m_material;
		class IAShaderPipeline *m_shaderPipeline;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;
	};
}
