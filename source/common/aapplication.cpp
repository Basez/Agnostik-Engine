#include "asharedh.hpp"
#include "aaplication.hpp"

#include "iarender_api.hpp"
#include "iawindow.hpp"
#include "iarenderer.hpp"
#include "iamesh.hpp"
#include "iashaderpipeline.hpp"
#include "iashader.hpp"

#include "amaterial.hpp"
#include "aentity.hpp"
#include "ascenemanager.hpp"
#include "aconfigmanager.hpp"
#include "aresourcemanager.hpp"
#include "adrawcommander.hpp"
#include "adrawcommand.hpp"

// shaders
// TODO: make crossplatform 
#include "shader_mesh_pix.hpp"
#include "shader_mesh_vert.hpp"
#include "shader_skybox_pix.hpp"
#include "shader_skybox_vert.hpp"

AGN::AAplication appTemp = AGN::AAplication();
AGN::AAplication& g_application = appTemp;

using namespace glm;

void AGN::AAplication::run(class IARenderAPI* a_renderAPI)
{
	m_renderAPI = a_renderAPI;
	m_quit = false;

	m_renderAPI->init();
	
	m_resourceManager = new AResourceManager(m_renderAPI->getDevice());

	m_drawCommander = new ADrawCommander();

	m_sceneManager = new ASceneManager();
	m_sceneManager->init();
	//m_sceneManager->loadTestScene01();
	m_sceneManager->loadScrambledScene();

	m_renderAPI->getRenderer().setCamera(m_sceneManager->getCurrentCamera());

	loadShaders();

	while (!m_quit)
	{
		update();

		// high level rendering
		createDrawQueue();
		m_drawCommander->sortCommandList();

		// api specific rendering
		m_renderAPI->getRenderer().render(*m_drawCommander);

		// clear render buckets at the end of the frame (after data is uploaded to the GPU)
		m_drawCommander->clearCommands();

		g_input.registerFrameCompletion();

		// api specific event handling, input etc
		m_renderAPI->handleEvents();
	}

}

void AGN::AAplication::cleanup()
{
	m_renderAPI = nullptr;
}

void AGN::AAplication::update()
{
	// calculate delatTime
	// TODO: Cross-Platform!
	static Uint32 lastTime = 0;
	Uint32 time = SDL_GetTicks();
	float deltaTime = float(time - lastTime) / 1000.0f;
	lastTime = time;

	// logic
	m_sceneManager->update(deltaTime);
}

void AGN::AAplication::loadShaders()
{
	// create shaders
	std::vector<AGN::IAShader*> meshShaders;
	meshShaders.push_back(&m_resourceManager->createShader(g_shader_mesh_vert, EAShaderType::VertexShader));
	meshShaders.push_back(&m_resourceManager->createShader(g_shader_mesh_pix, EAShaderType::PixelShader));
	m_meshShaderPipeline = &m_resourceManager->createShaderPipeline(meshShaders);

	// TODO: put back after uniform buffers are in
	//std::vector<AGN::IAShader*> skyboxShaders;
	//skyboxShaders.push_back(&resourceManager.createShader(g_shader_skybox_vert, EAShaderType::VertexShader));
	//skyboxShaders.push_back(&resourceManager.createShader(g_shader_skybox_pix, EAShaderType::PixelShader));
	//IAShaderPipeline& skyboxShaderPipeline = &resourceManager.createShaderPipeline(skyboxShaders);
}

void AGN::AAplication::createDrawQueue()
{
	// TODO: make these static draw commands
	// Swap buffer Draw command
	{
		// create sortkey
		uint8_t renderPhase = (uint8_t)RenderPhase::PostDraw;
		uint8_t layer = 0;
		uint8_t translucencyType = 0;
		uint8_t cmd = 0;
		uint16_t shaderPipelineId = 0;
		uint32_t meshId = 0;
		uint16_t materialId = 0;
		uint32_t depth = 0;
		uint64_t sortkey = ADrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

		ADrawCommand& drawCommand = m_drawCommander->addDrawCommand(EADrawCommandType::SwapBackBuffer, sortkey);
	}

	// TODO: make these static draw commands
	// PREDRAW clear buffer
	{
		// create sortkey
		uint8_t renderPhase = (uint8_t)RenderPhase::PreDraw;
		uint8_t layer = 0;
		uint8_t translucencyType = 0;
		uint8_t cmd = 0;
		uint16_t shaderPipelineId = 0;
		uint32_t meshId = 0;
		uint16_t materialId = 0;
		uint32_t depth = 0;
		uint64_t sortkey = ADrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);
		
		ADrawCommand& drawCommand = m_drawCommander->addDrawCommand(EADrawCommandType::ClearBuffer, sortkey);
		AClearBufferData& data = drawCommand.data.clearcolorData;
		data.buffersToClear = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT; // TODO: OpenGL specific? shouldnt be here
		data.clearColor = 0x00FF00;
	}

	// fill commander with skybox draw entity commands
	const std::vector<AEntity*> skyboxEntities = m_sceneManager->getSkyboxEntities();
	for (unsigned int i = 0; i < skyboxEntities.size(); i++)
	{
		AEntity& entity = *skyboxEntities[i];

		// create sortkey
		uint8_t renderPhase = (uint8_t)RenderPhase::FullscreenViewport;
		uint8_t layer = (uint8_t)RenderLayer::Skybox;
		uint8_t translucencyType = 0;				// TODO:
		uint8_t cmd = 0;							// TODO: ?
		uint16_t shaderPipelineId = m_meshShaderPipeline->getAId();
		uint32_t meshId = entity.getMesh()->getAId();
		uint16_t materialId = entity.getMaterial()->getAId();
		uint32_t depth = 0;							// TODO:

		uint64_t sortkey = ADrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

		ADrawCommand& drawCommand = m_drawCommander->addDrawCommand(EADrawCommandType::DrawEntity, sortkey);
		ADrawEntityData& data = drawCommand.data.entityData;
		data.entity = &entity;
		data.shaderPipeline = m_meshShaderPipeline;
	}

	// TODO: make these static draw commands
	// Clear Z Buffer after skybox
	{
		// create sortkey
		uint8_t renderPhase = (uint8_t)RenderPhase::FullscreenViewport;
		uint8_t layer = (uint8_t)RenderLayer::PreEntities;
		uint8_t translucencyType = 0;
		uint8_t cmd = 0;
		uint16_t shaderPipelineId = 0;
		uint32_t meshId = 0;
		uint16_t materialId = 0;
		uint32_t depth = 0;
		uint64_t sortkey = ADrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

		ADrawCommand& drawCommand = m_drawCommander->addDrawCommand(EADrawCommandType::ClearBuffer, sortkey);
		AClearBufferData& data = drawCommand.data.clearcolorData;
		data.buffersToClear = GL_DEPTH_BUFFER_BIT; // TODO: OpenGL specific? shouldnt be here
		data.clearColor = 0x00FF00;
	}

	// fill commander with entity draw entity commands
	const std::vector<AEntity*> entities = m_sceneManager->getEntities();
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		AEntity& entity = *entities[i];

		// create sortkey
		uint8_t renderPhase = (uint8_t)RenderPhase::FullscreenViewport;
		uint8_t layer = (uint8_t)RenderLayer::Entities;
		uint8_t translucencyType = 0;				// TODO:
		uint8_t cmd = 0;							// TODO: ?
		uint16_t shaderPipelineId = m_meshShaderPipeline->getAId();
		uint32_t meshId = entity.getMesh()->getAId();
		uint16_t materialId = entity.getMaterial()->getAId();
		uint32_t depth = 0;							// TODO:

		uint64_t sortkey = ADrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

		ADrawCommand& drawCommand = m_drawCommander->addDrawCommand(EADrawCommandType::DrawEntity, sortkey);
		ADrawEntityData& data = drawCommand.data.entityData;
		data.entity = &entity;
		data.shaderPipeline = m_meshShaderPipeline;
	}
}

AGN::IARenderAPI& AGN::AAplication::getRenderAPI()
{
	return dynamic_cast<AGN::IARenderAPI&>(*m_renderAPI);
}

AGN::AResourceManager& AGN::AAplication::getResourceManager()
{
	return *m_resourceManager;
}