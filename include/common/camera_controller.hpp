#pragma once

#include "i_input.hpp"

namespace AGN
{
	class CameraController : IInputMouseMotionHandler
	{
	public:
		CameraController(class Camera& a_camera, bool a_collideWithTerrain);
		~CameraController();

		void update(float a_deltaTime);
		void reset(glm::vec3 a_position, glm::vec2 a_xyRot);
		void onMouse(int button, int state, int x, int y);
		void setMovementSpeed(float a_slow, float a_normal, float a_fast);
		void onMouseMotion(int a_mouseX, int a_mouseY) override;

		glm::vec2 getXYRotationMovement() { return m_xyRotation - m_prevXYrotation; }

		void useYlimit(bool a_useYLimit, float a_yLimit) { m_useYLimit = a_useYLimit; m_yLimit = a_yLimit; }

	private:
		// rule of three
		CameraController(const CameraController& other) = delete;
		CameraController& operator=(const CameraController& other) = delete;

		class Camera& m_camera;
		bool m_collideWithTerrain;

		glm::ivec2 m_lastMousePosition;
		glm::ivec2 m_startDragPosition;
		glm::vec2 m_xyRotation;
		glm::vec2 m_prevXYrotation;
		float m_slowMovementSpeed;
		float m_normalMovementSpeed;
		float m_fastMovementSpeed;

		bool m_useYLimit;
		float m_yLimit;
	
	};
}