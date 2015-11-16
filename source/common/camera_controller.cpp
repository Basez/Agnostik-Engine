#include "shared.hpp"
#include "camera_controller.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "camera.hpp"
#include "i_window.hpp"
#include "application.hpp"
#include "i_render_api.hpp"

using namespace glm;

AGN::CameraController::CameraController(AGN::Camera& a_camera, bool a_collideWithTerrain)
	: m_camera(a_camera)
	, m_collideWithTerrain(a_collideWithTerrain)
	, m_xyRotation(glm::radians(-20.0f), 0.0f)
	, m_prevXYrotation(0, 0)
	, m_slowMovementSpeed(1.0f)
	, m_normalMovementSpeed(10.0f)
	, m_fastMovementSpeed(30.0f)
	, m_useYLimit(false)
	, m_yLimit(0.0f)
{
	g_input.addMouseMotionHandler(dynamic_cast<AGN::IInputMouseMotionHandler*>(this));
}

AGN::CameraController::~CameraController()
{
	g_input.removeMouseMotionHandler(dynamic_cast<AGN::IInputMouseMotionHandler*>(this));
}

void AGN::CameraController::setMovementSpeed(float a_slow, float a_normal, float a_fast)
{
	m_slowMovementSpeed = a_slow;
	m_normalMovementSpeed = a_normal;
	m_fastMovementSpeed = a_fast;
}

void AGN::CameraController::reset(glm::vec3 a_position, glm::vec2 a_xyRot)
{
	m_xyRotation = glm::vec2(glm::radians(a_xyRot.x), glm::radians(a_xyRot.y));

	quat rotation = quat();
	rotation = glm::rotate(rotation, m_xyRotation.y, vec3(0, 1, 0));
	rotation = glm::rotate(rotation, m_xyRotation.x, vec3(1, 0, 0));

	m_camera.reset(a_position, rotation);
}

void AGN::CameraController::update(float a_deltaTime)
{
	IWindow& window = g_application.getRenderAPI().getWindow();

	// mouse change
	// start / stop camera dragging state
	if (g_input.getMouseDown(EMOUSECODE::RIGHT))
	{
		window.showCursor(false);
		m_lastMousePosition = ivec2(g_input.getMouseX(), g_input.getMouseY());
		m_startDragPosition = m_lastMousePosition;
	}
	else if (g_input.getMouseUp(EMOUSECODE::RIGHT))
	{
		window.showCursor(true);
		window.warpCursor(m_startDragPosition);
		m_lastMousePosition = ivec2(g_input.getMouseX(), g_input.getMouseY());
	}

	// camera movement
	float cameraSpeed = m_normalMovementSpeed;

	if (g_input.getKey(ESCANCODE::LCTRL) || g_input.getKey(ESCANCODE::RCTRL)) cameraSpeed = m_slowMovementSpeed;
	if (g_input.getKey(ESCANCODE::LSHIFT) || g_input.getKey(ESCANCODE::RSHIFT)) cameraSpeed = m_fastMovementSpeed;

	bool localMovement = true;
	vec3 translation;

	int xMovement = (int)g_input.getKey(ESCANCODE::D) - (int)g_input.getKey(ESCANCODE::A);
	int yMovement = (int)g_input.getKey(ESCANCODE::SPACE) - (int)g_input.getKey(ESCANCODE::C);
	int zMovement = (int)g_input.getKey(ESCANCODE::S) - (int)g_input.getKey(ESCANCODE::W);

	if (localMovement)
	{
		translation = m_camera.getRotation() * glm::vec3(xMovement, yMovement, zMovement) * cameraSpeed * a_deltaTime;
	}
	else
	{
		translation = m_camera.getRotation() * glm::vec3(xMovement, yMovement, zMovement) * cameraSpeed * a_deltaTime;
	}

	if (m_useYLimit && m_camera.getPosition().y + translation.y < m_yLimit)
	{
		translation.y = 0;
		vec3 newPos = vec3(m_camera.getPosition().x, m_yLimit, m_camera.getPosition().z);
		m_camera.setPosition(newPos);
	}

	m_camera.translate(translation);
}

void AGN::CameraController::onMouse(int button, int state, int x, int y)
{


}

void AGN::CameraController::onMouseMotion(int a_mouseX, int a_mouseY)
{
	ivec2 currentMousePos = ivec2(a_mouseX, a_mouseY);
	vec2 delta = vec2(currentMousePos - m_lastMousePosition);
	m_lastMousePosition = currentMousePos;

	// is dragging state?
	if (g_input.getMouse(EMOUSECODE::RIGHT))
	{
		m_prevXYrotation = m_xyRotation;

		m_xyRotation.x += radians(-delta.y * 0.33f);
		m_xyRotation.y += radians(-delta.x * 0.33f);

		// limit 
		float maxRadians = (float)radians(90.0f);
		m_xyRotation.x = glm::clamp((float)m_xyRotation.x, -maxRadians, maxRadians);

		quat rotation = quat();
		rotation = glm::rotate(rotation, m_xyRotation.y, vec3(0, 1, 0));
		rotation = glm::rotate(rotation, m_xyRotation.x, vec3(1, 0, 0));

		m_camera.setRotation(rotation);

		IWindow& window = g_application.getRenderAPI().getWindow();

		// check if out of bounds
		const glm::ivec2 boundaryMargin = glm::ivec2(window.getDimensions().x / 5, window.getDimensions().y / 5);

		glm::ivec2 min = glm::ivec2(boundaryMargin.x, boundaryMargin.y);
		glm::ivec2 max = glm::ivec2(window.getDimensions().x - boundaryMargin.x, window.getDimensions().y - boundaryMargin.y);
		
		if (currentMousePos.x < min.x)
		{
			m_lastMousePosition = ivec2(max.x - 1, currentMousePos.y);
			window.warpCursor(m_lastMousePosition);
		}
		if (currentMousePos.x > max.x)
		{
			m_lastMousePosition = ivec2(min.x + 1, currentMousePos.y);
			window.warpCursor(m_lastMousePosition);
		}
		if (currentMousePos.y < min.y)
		{
			m_lastMousePosition = ivec2(currentMousePos.x, max.y - 1);
			window.warpCursor(m_lastMousePosition);
		}
		if (currentMousePos.y > max.y)
		{
			m_lastMousePosition = ivec2(currentMousePos.x, min.y + 1);
			window.warpCursor(m_lastMousePosition);
		}
	}
}