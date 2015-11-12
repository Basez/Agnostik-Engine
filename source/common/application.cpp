#include "shared.hpp"

#include <chrono>
#include <imgui/imgui.h>

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "i_input.hpp"
#include "i_render_api.hpp"
#include "i_window.hpp"
#include "i_renderer.hpp"
#include "i_mesh.hpp"
#include "i_shader_pipeline.hpp"
#include "i_shader.hpp"
#include "i_imgui.hpp"
#include "application.hpp"
#include "material.hpp"
#include "entity.hpp"
#include "scene_manager.hpp"
#include "config_manager.hpp"
#include "resource_manager.hpp"
#include "draw_commander.hpp"
#include "draw_command.hpp"
#include "camera.hpp"

// shaders
// TODO: make these shaders more cross-platform friendly.
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
	, m_sceneIndex(0)
	, m_sortEntityDrawCalls(true)
{

}

void AGN::Application::cleanup()
{
	delete m_sceneManager;
	delete m_drawCommander;
	delete m_resourceManager;
}

void AGN::Application::run(IRenderAPI* a_renderAPI)
{
	m_renderAPI = a_renderAPI;

	if (!m_renderAPI->init())
	{
		// Something went wrong with initialization
		return;
	}
	
	m_resourceManager = new ResourceManager(m_renderAPI->getDevice());
	m_resourceManager->loadDefaults();
	
	m_drawCommander = new DrawCommander();

	m_sceneIndex = g_configManager.getConfigPropertyAsInt32("start_scene_index");
	m_sceneManager = new SceneManager();
	m_sceneManager->init();

	m_renderAPI->getRenderer().setCamera(m_sceneManager->getCurrentCamera());

	bool doQuit = false;
	while (!doQuit)
	{
		if (m_sceneIndex != m_sceneManager->getCurrentSceneIndex())
		{
			// unload everything
			m_sceneManager->unloadScene();
			m_resourceManager->unloadAll();

			// load new stuff
			m_resourceManager->loadDefaults();
			loadShaders();
			m_sceneManager->loadScene(m_sceneIndex);
		}

		// api specific event handling, input etc
		m_renderAPI->handleEvents(doQuit);

		update();

		render();

		g_input.registerFrameCompletion();

		// if any window changes occurred
		if (m_renderAPI->getWindow().isDirty())
		{
			m_renderAPI->updateWindowState();
			m_sceneManager->onWindowUpdated(m_renderAPI->getWindow().getDimentions());
		}
	}
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

	m_renderAPI->getImGui().update(deltaTime);
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

void AGN::Application::renderGUI()
{
	static int32_t selectedSceneIndex = 0;
	static bool enableVsync = g_configManager.getConfigPropertyAsBool("vsync");
	
	ImGui::Begin("Config Menu", nullptr, ImGuiWindowFlags_NoSavedSettings);
	{
		// Select scene part
		ImGui::Text("Select Scene:");
		const char* sceneNames[] = { "Sponza", "Sibenik", "SuzannaCrate", "Empty" };
		ImGui::Combo("", &selectedSceneIndex, sceneNames, (int)(sizeof(sceneNames) / sizeof(*sceneNames)));
		if (ImGui::Button("Load"))
		{
			// set index to the selected scene index. Will load the scene next frame
			m_sceneIndex = selectedSceneIndex;
			g_log.debug("Load %s scene", sceneNames[selectedSceneIndex]);
		}

		ImGui::Separator();
		ImGui::Text("Configuration Settings:");
		
		// vsync checkbox & functionality
		bool prevEnableVsync = enableVsync;
		ImGui::Checkbox("Enable VSync", &enableVsync);
		if (prevEnableVsync != enableVsync) m_renderAPI->enableVSync(enableVsync);

		// sort draw commands
		ImGui::Checkbox("Sort mesh draw calls", &m_sortEntityDrawCalls);

		// FPS and other stats
		ImGui::Separator();
		ImGui::Text("Stats");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	// uncomment to test out ImGui
	//bool show_test_window = true;
	//ImGui::ShowTestWindow(&show_test_window);
}

void AGN::Application::loadShaders()
{
	// create shaders
	std::vector<AGN::IShader*> meshShaders;
	meshShaders.push_back(&m_resourceManager->createShader(g_shader_mesh_vert, EShaderType::VertexShader));
	meshShaders.push_back(&m_resourceManager->createShader(g_shader_mesh_pix, EShaderType::PixelShader));
	m_meshShaderPipeline = &m_resourceManager->createShaderPipeline(meshShaders);
	
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

	if (m_renderAPI->getImGui().isEnabled()) renderGUI();
	
	// api specific rendering, parses command list
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

	// TODO: make static
	// Draw GUI command
	{
		uint8_t renderPhase = (uint8_t)ERenderPhase::GUIViewport;
		uint8_t layer = 0;
		uint8_t translucencyType = 0;
		uint8_t cmd = 0;
		uint16_t shaderPipelineId = 0;
		uint32_t meshId = 0;
		uint16_t materialId = 0;
		uint32_t depth = 0;
		uint64_t sortkey = DrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

		DrawCommand& drawCommand = m_drawCommander->addDrawCommand(EDrawCommandType::DrawGUI, sortkey);
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

		MeshCollection* entityMeshCollection = entity.getMeshCollection();
		for (unsigned int j = 0; j < entityMeshCollection->getNumMeshes(); j++)
		{
			IMesh* mesh = entityMeshCollection->getMeshList()[j];
			Material* material = mesh->getMaterial();

			// create sortkey
			uint8_t renderPhase = (uint8_t)ERenderPhase::FullscreenViewport;
			uint8_t layer = (uint8_t)ERenderLayer::Skybox;
			uint8_t translucencyType = 0;				// TODO:
			uint8_t cmd = 0;							// TODO: ?
			uint16_t shaderPipelineId = m_sortEntityDrawCalls ? m_skyboxShaderPipeline->getAId() : 0;
			uint32_t meshId = m_sortEntityDrawCalls ? mesh->getAId() : 0;
			uint16_t materialId = m_sortEntityDrawCalls ? material->getAId() : 0;
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

		MeshCollection* entityMeshCollection = entity.getMeshCollection();
		for (unsigned int j = 0; j < entityMeshCollection->getNumMeshes(); j++, depthVectorIndex++)
		{
			IMesh* mesh = entityMeshCollection->getMeshList()[j];
			Material* material = mesh->getMaterial();
			 
			// create sortkey
			uint8_t renderPhase = (uint8_t)ERenderPhase::FullscreenViewport;
			uint8_t layer = (uint8_t)ERenderLayer::Entities;
			uint8_t translucencyType = 0;				// TODO:
			uint8_t cmd = 0;							// TODO: ?
			uint16_t shaderPipelineId = m_sortEntityDrawCalls ? m_meshShaderPipeline->getAId() : 0;
			uint32_t meshId = m_sortEntityDrawCalls ? mesh->getAId() : 0;
			uint16_t materialId = m_sortEntityDrawCalls ? material->getAId() : 0;
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