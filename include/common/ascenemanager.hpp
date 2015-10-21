#pragma once 

namespace AGN
{
	class ASceneManager
	{
	public:
		ASceneManager();
		~ASceneManager();

		void init();
		void update(float a_deltaTime);
		void loadScene();

		const std::vector<class AEntity*> getEntities() const { return m_entities; }
		const std::vector<class AEntity*> getSkyboxEntities() const { return m_skyboxEntities; }
		void onWindowUpdated(glm::ivec2 a_dimentions);
		class ACamera* getCurrentCamera() { return m_camera; }

	private:
		void loadScrambledScene();
		void loadSponza();

		std::vector<class AEntity*> m_entities; 
		std::vector<class AEntity*> m_skyboxEntities;
		class ACamera* m_camera;
		class ACameraController* m_cameraController;
	};
}