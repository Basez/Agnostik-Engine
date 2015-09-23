#pragma once

#include "iainput.hpp"

namespace AGN
{
	class ACameraController : IAInputMouseMotionHandler
	{
	public:
		ACameraController(class ACamera& a_camera, bool a_collideWithTerrain);
		~ACameraController();

		void update(float a_deltaTime);
		void reset();
		void onMouse(int button, int state, int x, int y);
		void setMovementSpeed(float a_slow, float a_normal, float a_fast);
		void onMouseMotion(int a_mouseX, int a_mouseY) override;

		glm::vec2 getXYRotationMovement() { return m_xyRotation - m_prevXYrotation; }

		void useYlimit(bool a_useYLimit, float a_yLimit) { m_useYLimit = a_useYLimit; m_yLimit = a_yLimit; }

	private:
		// rule of three
		ACameraController(const ACameraController& other) = delete;
		ACameraController& operator=(const ACameraController& other) = delete;

		class ACamera& m_camera;
		bool m_collideWithTerrain;

		glm::ivec2 m_lastMousePosition;
		glm::vec2 m_xyRotation;
		glm::vec2 m_prevXYrotation;
		float m_slowMovementSpeed;
		float m_normalMovementSpeed;
		float m_fastMovementSpeed;

		bool m_useYLimit;
		float m_yLimit;
	
	};
}