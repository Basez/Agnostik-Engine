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
		void addNode();
	};
}