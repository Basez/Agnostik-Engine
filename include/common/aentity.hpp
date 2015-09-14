#pragma once

namespace AGN
{
	class AEntity
	{
	public:
		AEntity();

		void setMesh(class IAMesh* a_mesh);
		void setShaderProgram(class IAShaderProgram* a_shaderProgram);
		void addTexture(class IATexture* a_texture);

		void setPosition(glm::vec3 a_position) { m_position = a_position; }
		void setRotation(glm::quat a_rotation) { m_rotation = a_rotation; }
		void setScale(glm::vec3 a_scale) { m_scale = a_scale; }

	private:
		class IAMesh *m_mesh;
		class IAShaderProgram *m_shaderProgram;
		std::vector<class IATexture*> m_textures;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;
	};
}
