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

		const std::vector<class AEntity*> getEntities() const { return m_entities; }
		class ACamera* getCurrentCamera() { return m_camera; }

	private:
		std::vector<class AEntity*> m_entities; 
		class ACamera* m_camera;
		class ACameraController* m_cameraController;
	};
}