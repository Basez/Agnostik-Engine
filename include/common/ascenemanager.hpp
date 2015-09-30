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
		void loadTestScene01();
		void loadScrambledScene();
		void loadSponza();

		const std::vector<class AEntity*> getEntities() const { return m_entities; }
		const std::vector<class AEntity*> getSkyboxEntities() const { return m_skyboxEntities; }

		class ACamera* getCurrentCamera() { return m_camera; }

	private:
		std::vector<class AEntity*> m_entities; 
		std::vector<class AEntity*> m_skyboxEntities;

		class ACamera* m_camera;
		class ACameraController* m_cameraController;
	};
}