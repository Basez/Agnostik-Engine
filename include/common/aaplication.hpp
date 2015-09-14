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
		void fetchRender();

		class IARenderAPI* m_renderAPI;
		class ASceneManager* m_sceneManager;
		class AResourceManager* m_resourceManager;

		bool m_quit;
	};



}
extern AGN::AAplication& g_application;