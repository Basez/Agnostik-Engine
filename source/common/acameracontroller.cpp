#include "asharedh.hpp"
#include "acameracontroller.hpp"
#include "acamera.hpp"
#include "iawindow.hpp"
#include "aaplication.hpp"
#include "iarender_api.hpp"

using namespace glm;

AGN::ACameraController::ACameraController(AGN::ACamera& a_camera, bool a_collideWithTerrain)
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
	g_input.addMouseMotionHandler(dynamic_cast<AGN::IAInputMouseMotionHandler*>(this));
}

AGN::ACameraController::~ACameraController()
{
	g_input.removeMouseMotionHandler(dynamic_cast<AGN::IAInputMouseMotionHandler*>(this));
}

void AGN::ACameraController::setMovementSpeed(float a_slow, float a_normal, float a_fast)
{
	m_slowMovementSpeed = a_slow;
	m_normalMovementSpeed = a_normal;
	m_fastMovementSpeed = a_fast;
}

void AGN::ACameraController::reset()
{
	m_camera.reset();
	m_xyRotation = vec2(0.0f, 0.0f);
}

void AGN::ACameraController::update(float a_deltaTime)
{
	IAWindow& window = g_application.getRenderAPI().getWindow();

	// mouse change
	// start / stop camera dragging state
	if (g_input.getMouseDown(AGN_MOUSECODE::RIGHT))
	{
		window.showCursor(false);
		m_lastMousePosition = ivec2(g_input.getMouseX(), g_input.getMouseY());
	}
	else if (g_input.getMouseUp(AGN_MOUSECODE::RIGHT))
	{
		window.showCursor(true);
		m_lastMousePosition = ivec2(g_input.getMouseX(), g_input.getMouseY());
	}

	// camera movement
	float cameraSpeed = m_normalMovementSpeed;

	if (g_input.getKey(SCANCODE::LCTRL) || g_input.getKey(SCANCODE::RCTRL)) cameraSpeed = m_slowMovementSpeed;
	if (g_input.getKey(SCANCODE::LSHIFT) || g_input.getKey(SCANCODE::RSHIFT)) cameraSpeed = m_fastMovementSpeed;

	bool localMovement = true;
	vec3 translation;

	int xMovement = (int)g_input.getKey(SCANCODE::D) - (int)g_input.getKey(SCANCODE::A);
	int yMovement = (int)g_input.getKey(SCANCODE::SPACE) - (int)g_input.getKey(SCANCODE::C);
	int zMovement = (int)g_input.getKey(SCANCODE::S) - (int)g_input.getKey(SCANCODE::W);

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

void AGN::ACameraController::onMouse(int button, int state, int x, int y)
{


}

void AGN::ACameraController::onMouseMotion(int a_mouseX, int a_mouseY)
{
	ivec2 currentMousePos = ivec2(a_mouseX, a_mouseY);
	vec2 delta = vec2(currentMousePos - m_lastMousePosition);
	m_lastMousePosition = currentMousePos;

	// is dragging state?
	if (g_input.getMouse(AGN_MOUSECODE::RIGHT))
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

		IAWindow& window = g_application.getRenderAPI().getWindow();

		// warp mouse to center of screen, for infinite dragging
		m_lastMousePosition = ivec2(window.getDimentions().x >> 1, window.getDimentions().y >> 1);
		window.warpCursor(m_lastMousePosition);
	}
}