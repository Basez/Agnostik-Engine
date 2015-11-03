#pragma once 

namespace AGN
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		void init();
		void update(float a_deltaTime);
		void loadScene();

		const std::vector<class Entity*> getEntities() const { return m_entities; }
		const std::vector<class Entity*> getSkyboxEntities() const { return m_skyboxEntities; }
		void onWindowUpdated(glm::ivec2 a_dimentions);
		class Camera* getCurrentCamera() { return m_camera; }

	private:
		void loadScrambledScene();
		void loadSponza();

		std::vector<class Entity*> m_entities; 
		std::vector<class Entity*> m_skyboxEntities;
		class Camera* m_camera;
		class CameraController* m_cameraController;
	};
}