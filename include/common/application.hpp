#pragma once

namespace AGN
{
	class Application
	{
	public:
		Application();
		void cleanup();

		void run(class IRenderAPI* a_renderAPI);
		class IRenderAPI& getRenderAPI();
		class ResourceManager& getResourceManager();

	private:
		void update();
		void render();
		void renderGUIElements();
		void createDrawQueue();
		void loadShaders();
		void updateMeshShaderProperties(float a_deltaTime);

		class IRenderAPI* m_renderAPI;
		class SceneManager* m_sceneManager;
		class ResourceManager* m_resourceManager;
		class DrawCommander* m_drawCommander;
		class IShaderPipeline* m_meshShaderPipeline;
		class IShaderPipeline* m_skyboxShaderPipeline;
		int32_t m_sceneIndex;

	};
}
extern AGN::Application& g_application;

