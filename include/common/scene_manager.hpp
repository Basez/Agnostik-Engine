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
		void loadScene(int16_t a_index);
		void unloadScene();

		const std::vector<class Entity*> getEntities() const { return m_entities; }
		const std::vector<class Entity*> getSkyboxEntities() const { return m_skyboxEntities; }
		void onWindowUpdated(glm::ivec2 a_dimentions);
		class Camera* getCurrentCamera() { return m_camera; }
		int16_t getCurrentSceneIndex() { return m_currentSceneIndex; }
	
	private:
		void loadSponza();
		void loadSibenik();
		void loadSuzannaCrate();
		void loadEmpty();
		std::vector<class Entity*> m_entities; 
		std::vector<class Entity*> m_skyboxEntities;
		class Camera* m_camera;
		class CameraController* m_cameraController;
		int16_t m_currentSceneIndex;
	};
}