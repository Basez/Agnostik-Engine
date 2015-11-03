#include "shared.hpp"
#include "camera.hpp"
#include "application.hpp"
#include "i_render_api.hpp"
#include "i_window.hpp"

using namespace glm;

AGN::Camera::Camera(glm::vec3 a_initialPosition, glm::quat a_initialRotation)
	: m_initialPosition(a_initialPosition)
	, m_initialRotation(a_initialRotation)
	, m_position(a_initialPosition)
	, m_rotation(a_initialRotation)
	, m_viewMatrix(glm::mat4())
	, m_projectionMatrix(glm::mat4())
	, m_hasChangedFlag(true)
{
	
}

void AGN::Camera::reset()
{
	m_position = m_initialPosition;
	m_rotation = m_initialRotation;
	m_hasChangedFlag = true;
}

void AGN::Camera::applyViewMatrix()
{
	updateViewMatrix();
}

void AGN::Camera::updateViewMatrix()
{
	if (m_hasChangedFlag)
	{
		mat4 translate, rotate, scale;

		translate = glm::translate(vec3(-m_position.x, -m_position.y, -m_position.z));
		rotate = transpose(toMat4(m_rotation));
		scale = glm::scale(vec3(1, 1, 1));
		
		m_viewMatrix = rotate * translate * scale;

		m_hasChangedFlag = false;
	}
}

void AGN::Camera::translate(glm::vec3& a_addedPosition)
{
	m_position += a_addedPosition;
	m_hasChangedFlag = true;
}

void AGN::Camera::rotate(glm::quat& a_addedRotation)
{
	m_rotation = m_rotation * a_addedRotation;
	m_hasChangedFlag = true;
}

void AGN::Camera::setPosition(glm::vec3& a_postion)
{
	m_position = a_postion;
	m_hasChangedFlag = true;
}

void AGN::Camera::setRotation(glm::quat& a_rotation)
{
	m_rotation = a_rotation;
	m_hasChangedFlag = true;
}

void AGN::Camera::setEulerAngles(glm::vec3& a_eulerAngles)
{
	m_rotation = quat(glm::radians(a_eulerAngles));
	m_hasChangedFlag = true;
}

void AGN::Camera::setProjectionRH(float a_fov, float a_zNear, float a_zFar)
{
	ivec2 windowDimentions = g_application.getRenderAPI().getWindow().getDimentions();
	float aspectRatio = (float)windowDimentions.x / (float)windowDimentions.y;

	m_projectionMatrix = glm::perspective(glm::radians(a_fov), aspectRatio, a_zNear, a_zFar);
	m_hasChangedFlag = true;
}

const glm::vec3 AGN::Camera::getForward() const
{
	mat4 rotate = transpose(toMat4(m_rotation));
	vec4 fwd = vec4(0, 0, 1, 0) * rotate;

	return vec3(fwd.x, fwd.y, fwd.z);
}

const glm::vec3 AGN::Camera::getEulerAngles() const
{
	return degrees(eulerAngles(m_rotation));
}

const glm::mat4 AGN::Camera::getViewMatrix()
{
	updateViewMatrix();
	return m_viewMatrix;
}

const glm::vec3 AGN::Camera::getWorldPositionFromScreenPosition(const glm::vec2 a_screenPos) const
{
	g_log.error("TODO: Implement: ACamera::getWorldPositionFromScreenPosition()");
	return vec3(-1);
}
