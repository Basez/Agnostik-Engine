#pragma once

namespace AGN
{
	class AAplication
	{
	public:
		void run(class IARenderAPI* a_renderAPI);
		void cleanup();

		class IARenderAPI& getRenderAPI();
		class AResourceManager& getResourceManager();

	private:
		void update();
		void createDrawQueue();
		void sortDrawQueue();

		class IARenderAPI* m_renderAPI;
		class ASceneManager* m_sceneManager;
		class AResourceManager* m_resourceManager;
		class ADrawCommander* m_drawCommander;

		bool m_quit;
	};



}
extern AGN::AAplication& g_application;