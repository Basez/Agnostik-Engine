#pragma once 

namespace AGN
{
	class ASceneManager
	{
	public:
		ASceneManager();
		~ASceneManager();
		void init();
		void loadTestScene01();

	private:
		std::vector<class AEntity*> m_entities; 
		class ACamera* m_camera;
		
	};
}