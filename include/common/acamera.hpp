#pragma once

namespace AGN
{
	class ACamera
	{
	public:
		ACamera(glm::vec3 a_initialPosition = glm::vec3(), glm::quat a_initialRotation = glm::quat());

		void reset();
		void applyViewMatrix();
		void translate(glm::vec3& a_addedPosition);

		void rotate(glm::quat& a_addedRotation);
		void setPosition(glm::vec3& a_postion);
		void setRotation(glm::quat& a_rotation);
		void setEulerAngles(glm::vec3& a_eulerAngles);
		void setProjectionRH(float a_fov, float a_zNear, float a_zFar);

		const glm::vec3 getPosition() const { return m_position; }
		const glm::quat getRotation() const { return m_rotation; }
		const glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
		const glm::vec3 getForward() const;
		const glm::vec3 getEulerAngles() const;
		const glm::mat4 getViewMatrix();
		const glm::vec3 getWorldPositionFromScreenPosition(const glm::vec2 a_screenPos) const;

	private:
		void updateViewMatrix();

		glm::vec3 m_initialPosition;
		glm::quat m_initialRotation;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		bool m_hasChangedFlag;
	};
}




	
	



