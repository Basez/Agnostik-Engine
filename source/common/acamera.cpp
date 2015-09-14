#include "asharedh.hpp"
#include "acamera.hpp"
#include "aaplication.hpp"
#include "iarender_api.hpp"
#include "iawindow.hpp"

using namespace glm;

AGN::ACamera::ACamera()
	: m_position(glm::vec3())
	, m_rotation(glm::quat())
	, m_viewMatrix(glm::mat4())
	, m_projectionMatrix(glm::mat4())
	, m_hasChangedFlag(true)
{
	
}

void AGN::ACamera::reset()
{
	m_rotation = quat();
	m_hasChangedFlag = true;
}

void AGN::ACamera::applyViewMatrix()
{
	updateViewMatrix();
}

void AGN::ACamera::updateViewMatrix()
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

void AGN::ACamera::translate(const glm::vec3& a_addedPosition)
{
	m_position += a_addedPosition;
	m_hasChangedFlag = true;
}

void AGN::ACamera::rotate(const glm::quat& a_addedRotation)
{
	m_rotation = m_rotation * a_addedRotation;
	m_hasChangedFlag = true;
}

void AGN::ACamera::setPosition(const glm::vec3& a_postion)
{
	m_position = a_postion;
	m_hasChangedFlag = true;
}

void AGN::ACamera::setRotation(const glm::quat& a_rotation)
{
	m_rotation = a_rotation;
	m_hasChangedFlag = true;
}

void AGN::ACamera::setEulerAngles(const glm::vec3& a_eulerAngles)
{
	m_rotation = quat(glm::radians(a_eulerAngles));
	m_hasChangedFlag = true;
}

void AGN::ACamera::setProjectionRH(float a_fov, float a_zNear, float a_zFar)
{
	ivec2 windowDimentions = g_application.getRenderAPI().getWindow().getDimentions();
	float aspectRatio = (float)windowDimentions.x / (float)windowDimentions.y;

	m_projectionMatrix = glm::perspective(glm::radians(a_fov), aspectRatio, a_zNear, a_zFar);
}

const glm::vec3 AGN::ACamera::getForward() const
{
	mat4 rotate = transpose(toMat4(m_rotation));
	vec4 fwd = vec4(0, 0, 1, 0) * rotate;

	return vec3(fwd.x, fwd.y, fwd.z);
}

const glm::vec3 AGN::ACamera::getEulerAngles() const
{
	return degrees(eulerAngles(m_rotation));
}

const glm::mat4 AGN::ACamera::getViewMatrix()
{
	updateViewMatrix();
	return m_viewMatrix;
}

const glm::vec3 AGN::ACamera::getWorldPositionFromScreenPosition(const glm::vec2 a_screenPos) const
{
	g_log.error("TODO: Implement: ACamera::getWorldPositionFromScreenPosition()");
	return vec3(-1);
}
