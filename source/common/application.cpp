#include "shared.hpp"
#include "i_input.hpp"
#include "i_render_api.hpp"
#include "i_window.hpp"
#include "i_renderer.hpp"
#include "i_mesh.hpp"
#include "i_shader_pipeline.hpp"
#include "i_shader.hpp"
#include "application.hpp"
#include "material.hpp"
#include "entity.hpp"
#include "scene_manager.hpp"
#include "config_manager.hpp"
#include "resource_manager.hpp"
#include "draw_commander.hpp"
#include "draw_command.hpp"
#include "camera.hpp"

#include <chrono>

// shaders
// TODO: make these shaders more crossplatform friendly.
// TODO: Perhaps let the pre-build step figure it out or generate both types in the same file, with a special getter
#include "shader_mesh_pix.hpp"
#include "shader_mesh_vert.hpp"
#include "shader_skybox_pix.hpp"
#include "shader_skybox_vert.hpp"

AGN::Application appTemp = AGN::Application();
AGN::Application& g_application = appTemp;

using namespace glm;

AGN::Application::Application()
	: m_meshShaderPipeline(nullptr)
	, m_skyboxShaderPipeline(nullptr)
{

}

void AGN::Application::run(class IRenderAPI* a_renderAPI)
{
	m_renderAPI = a_renderAPI;

	if (!m_renderAPI->init())
	{
		// Something went wrong with initialization
		return;
	}
	
	m_resourceManager = new ResourceManager(m_renderAPI->getDevice());
	m_resourceManager->loadDefaults();
	loadShaders();

	m_drawCommander = new DrawCommander();

	m_sceneManager = new SceneManager();
	m_sceneManager->init();
	m_sceneManager->loadScene();

	m_renderAPI->getRenderer().setCamera(m_sceneManager->getCurrentCamera());

	bool doQuit = false;
	while (!doQuit)
	{
		update();

		render();

		g_input.registerFrameCompletion();

		// log currently alive objects in gpu memory (currently only working for DX11)
		//m_renderAPI->logLiveObjects();

		// api specific event handling, input etc
		m_renderAPI->handleEvents(doQuit);

		// if any window changes occured
		// TODO: rethink this?
		if (m_renderAPI->getWindow().isDirty())
		{
			m_renderAPI->getWindow().updateWindowState();
			m_renderAPI->getRenderer().onWindowUpdated(m_renderAPI->getWindow().getDimentions());
			m_sceneManager->onWindowUpdated(m_renderAPI->getWindow().getDimentions());
		}
	}
}

void AGN::Application::cleanup()
{
	delete m_sceneManager;
	delete m_drawCommander;
	delete m_resourceManager;
}

void AGN::Application::update()
{
	// calculate delatTime
	static std::chrono::steady_clock clock;
	static auto lastTime = clock.now();
	auto currentTime = clock.now();
	std::chrono::duration<float> timeDifference = currentTime - lastTime;
	float deltaTime = timeDifference.count();
	lastTime = currentTime;
	
	// limit deltatime
	if (deltaTime > 1.0f) deltaTime = 1.0f;

	// logic
	m_sceneManager->update(deltaTime);
	if (m_meshShaderPipeline) updateMeshShaderProperties(deltaTime);
}

void AGN::Application::updateMeshShaderProperties(float a_deltaTime)
{
	// change mesh light properties
	// test code to update shader buffer
	// TODO: Abstract this, currently very much hardcoded
	unsigned char buffer[48] = {0};

	static float rotationY = 0.0f;
	rotationY += a_deltaTime * 45.0f;
	
	const vec3 lightDirectionNorm = glm::rotate(normalize(vec3(0.5f, 1.0f, 0.5f)), glm::radians(rotationY), vec3(0, 1, 0));

	//g_log.debug("lightDirectionNorm: x:%f - y:%f - z:%f", lightDirectionNorm.x, lightDirectionNorm.y, lightDirectionNorm.z);

	float lightDirection[4] = { lightDirectionNorm.x, lightDirectionNorm.y, lightDirectionNorm.z, 0.0f };
	float lightColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightAmbient[4] = { 0.2f, 0.2f, 0.2f, 0.2f };

	// TODO: get buffer offset?
	memcpy(buffer + 0, lightDirection, 4 * sizeof(float));
	memcpy(buffer + 16, lightColor, 4 * sizeof(float));
	memcpy(buffer + 32, lightAmbient, 4 * sizeof(float));

	m_meshShaderPipeline->setConstantBufferData(EShaderType::PixelShader,"LightSettings", &buffer, 48);
}

void AGN::Application::loadShaders()
{
	// create shaders
	std::vector<AGN::IShader*> meshShaders;
	meshShaders.push_back(&m_resourceManager->createShader(g_shader_mesh_vert, EShaderType::VertexShader));
	meshShaders.push_back(&m_resourceManager->createShader(g_shader_mesh_pix, EShaderType::PixelShader));
	m_meshShaderPipeline = &m_resourceManager->createShaderPipeline(meshShaders);
	
	// TODO: RESTORE after HLSL
	std::vector<AGN::IShader*> skyboxShaders;
	skyboxShaders.push_back(&m_resourceManager->createShader(g_shader_skybox_vert, EShaderType::VertexShader));
	skyboxShaders.push_back(&m_resourceManager->createShader(g_shader_skybox_pix, EShaderType::PixelShader));
	m_skyboxShaderPipeline = &m_resourceManager->createShaderPipeline(skyboxShaders);
	
}

void AGN::Application::render()
{
	// high level rendering
	createDrawQueue();
	m_drawCommander->sortCommandList();

	// api specific rendering
	m_renderAPI->getRenderer().render(*m_drawCommander);

	// clear render buckets at the end of the frame (after data is uploaded to the GPU)
	m_drawCommander->clearCommands();
}

void AGN::Application::createDrawQueue()
{
	// TODO: make these static draw commands
	// Swap buffer Draw command
	{
		// create sortkey
		uint8_t renderPhase = (uint8_t)ERenderPhase::PostDraw;
		uint8_t layer = 0;
		uint8_t translucencyType = 0;
		uint8_t cmd = 0;
		uint16_t shaderPipelineId = 0;
		uint32_t meshId = 0;
		uint16_t materialId = 0;
		uint32_t depth = 0;
		uint64_t sortkey = DrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

		DrawCommand& drawCommand = m_drawCommander->addDrawCommand(EDrawCommandType::SwapBackBuffer, sortkey);
	}

	// TODO: make these static draw commands
	// PREDRAW clear buffer
	{
		uint8_t renderPhase = (uint8_t)ERenderPhase::PreDraw;
		uint8_t layer = 0;
		uint8_t translucencyType = 0;
		uint8_t cmd = 0;
		uint16_t shaderPipelineId = 0;
		uint32_t meshId = 0;
		uint16_t materialId = 0;
		uint32_t depth = 0;
		uint64_t sortkey = DrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);
		
		DrawCommand& drawCommand = m_drawCommander->addDrawCommand(EDrawCommandType::ClearBuffer, sortkey);
		ClearBufferData& data = drawCommand.data.clearBufferData;
		data.buffersToClear = (uint32_t)EDrawBufferType::COLOR | (uint32_t)EDrawBufferType::DEPTH;
		data.clearColor = 0x00FF0000;
	}

	const std::vector<Entity*> skyboxEntities = m_sceneManager->getSkyboxEntities();
	for (unsigned int i = 0; i < skyboxEntities.size(); i++)
	{
		Entity& entity = *skyboxEntities[i];

		// calculate model matrix
		mat4 translation = glm::translate(entity.getPosition());
		mat4 rotation = toMat4(entity.getRotation());
		mat4 scaling = scale(entity.getScale());
		mat4 modelMatrix = translation * rotation * scaling;

		for (unsigned int j = 0; j < entity.getMeshCollection().size(); j++)
		{
			IMesh* mesh = entity.getMeshCollection()[j];
			Material* material = mesh->getMaterial();

			// create sortkey
			uint8_t renderPhase = (uint8_t)ERenderPhase::FullscreenViewport;
			uint8_t layer = (uint8_t)ERenderLayer::Skybox;
			uint8_t translucencyType = 0;				// TODO:
			uint8_t cmd = 0;							// TODO: ?
			uint16_t shaderPipelineId = m_skyboxShaderPipeline->getAId();
			uint32_t meshId = mesh->getAId();
			uint16_t materialId = material->getAId();
			uint32_t depth = 0;							// TODO:
			uint64_t sortkey = DrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

			DrawCommand& drawCommand = m_drawCommander->addDrawCommand(EDrawCommandType::DrawEntity, sortkey);
			DrawEntityData& data = drawCommand.data.entityData;
			data.shaderPipeline = m_skyboxShaderPipeline;
			data.material = material;
			data.mesh = mesh;

			// store matrix as float array (for the union data type to work)
			memcpy(&data.modelMatrixArray[0], glm::value_ptr(modelMatrix), sizeof(data.modelMatrixArray));
		}
	}
	
	// TODO: make these static draw commands
	// Clear Z Buffer after skybox
	{
		uint8_t renderPhase = (uint8_t)ERenderPhase::FullscreenViewport;
		uint8_t layer = (uint8_t)ERenderLayer::PreEntities;
		uint8_t translucencyType = 0;
		uint8_t cmd = 0;
		uint16_t shaderPipelineId = 0;
		uint32_t meshId = 0;
		uint16_t materialId = 0;
		uint32_t depth = 0;
		uint64_t sortkey = DrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

		DrawCommand& drawCommand = m_drawCommander->addDrawCommand(EDrawCommandType::ClearBuffer, sortkey);
		ClearBufferData& data = drawCommand.data.clearBufferData;
		data.buffersToClear = (uint32_t)EDrawBufferType::DEPTH;
		data.clearColor = 0x000000;
	}

	/*
	// get depths of every entity
	std::vector<float> depthVector;
	float maxDepth = 0.0f;
	glm::vec3 camPosition = m_sceneManager->getCurrentCamera()->getPosition();
	const uint32_t maxDepthEntries = static_cast<uint32_t>(1) << static_cast<uint32_t>(ADrawCommander::SortKeyBitAmount::Depth);

	const std::vector<AEntity*> entities = m_sceneManager->getEntities();
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		const AEntity& entity = *entities[i];
		for (unsigned int j = 0; j < entity.getMeshCollection().size(); j++)
		{
			IAMesh& mesh = *entity.getMeshCollection()[j];

			// calculate depth to camera
			const glm::vec3 diff = (entities[i]->getPosition() + mesh.getCenterPoint()) - camPosition;
			float dp = glm::dot(diff, diff);
			depthVector.push_back(dp);

			if (dp > maxDepth) maxDepth = dp;
		}
	}*/

	
	// fill commander with entity draw entity commands
	const std::vector<Entity*> entities = m_sceneManager->getEntities();
	for (unsigned int i = 0, depthVectorIndex = 0; i < entities.size(); i++)
	{
		Entity& entity = *entities[i];
		
		// calculate model matrix
		mat4 translation = glm::translate(entity.getPosition());
		mat4 rotation = toMat4(entity.getRotation());
		mat4 scaling = scale(entity.getScale());
		mat4 modelMatrix = translation * rotation * scaling;

		for (unsigned int j = 0; j < entity.getMeshCollection().size(); j++, depthVectorIndex++)
		{
			IMesh* mesh = entity.getMeshCollection()[j];
			Material* material = mesh->getMaterial();
			 
			// create sortkey
			uint8_t renderPhase = (uint8_t)ERenderPhase::FullscreenViewport;
			uint8_t layer = (uint8_t)ERenderLayer::Entities;
			uint8_t translucencyType = 0;				// TODO:
			uint8_t cmd = 0;							// TODO: ?
			uint16_t shaderPipelineId = m_meshShaderPipeline->getAId();
			uint32_t meshId = mesh->getAId();
			uint16_t materialId = material->getAId();
			uint32_t depth = 0;//(depthVector[depthVectorIndex] / (maxDepth+1)) * maxDepthEntries; // add sorted depth
			uint64_t sortkey = DrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);
			
			DrawCommand& drawCommand = m_drawCommander->addDrawCommand(EDrawCommandType::DrawEntity, sortkey);
			DrawEntityData& data = drawCommand.data.entityData;
			data.shaderPipeline = m_meshShaderPipeline;
			data.material = material;
			data.mesh = mesh;

			// store matrix as float array (for the union data type to work)
			memcpy(&data.modelMatrixArray[0], glm::value_ptr(modelMatrix), sizeof(data.modelMatrixArray));
		}
	}
	
}

AGN::IRenderAPI& AGN::Application::getRenderAPI()
{
	return dynamic_cast<AGN::IRenderAPI&>(*m_renderAPI);
}

AGN::ResourceManager& AGN::Application::getResourceManager()
{
	return *m_resourceManager;
}