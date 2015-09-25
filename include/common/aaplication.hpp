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
		void quit() { m_quit = true; }

	private:
		void update();
		void render();
		void createDrawQueue();
		void loadShaders();
		void updateMeshShaderProperties(float a_deltaTime);

		class IARenderAPI* m_renderAPI;
		class ASceneManager* m_sceneManager;
		class AResourceManager* m_resourceManager;
		class ADrawCommander* m_drawCommander;

		class IAShaderPipeline* m_meshShaderPipeline;
		class IAShaderPipeline* m_skyboxShaderPipeline;

		bool m_quit;
	};
}
extern AGN::AAplication& g_application;

