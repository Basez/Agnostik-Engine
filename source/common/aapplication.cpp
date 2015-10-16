#include "asharedh.hpp"
#include "aaplication.hpp"
#include <chrono>

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
#include "acamera.hpp"

// shaders
// TODO: find crossplatform for these shaders. Perhaps let the pre-build step figure it out or generate both types in the same file?
#include "shader_mesh_pix.hpp"
#include "shader_mesh_vert.hpp"
#include "shader_skybox_pix.hpp"
#include "shader_skybox_vert.hpp"

AGN::AAplication appTemp = AGN::AAplication();
AGN::AAplication& g_application = appTemp;

using namespace glm;


AGN::AAplication::AAplication()
	: m_meshShaderPipeline(nullptr)
	, m_skyboxShaderPipeline(nullptr)
{

}

void AGN::AAplication::run(class IARenderAPI* a_renderAPI)
{
	m_renderAPI = a_renderAPI;
	m_quit = false;

	if (!m_renderAPI->init())
	{
		// Something went wrong with initialization
		return;
	}
	
	m_resourceManager = new AResourceManager(m_renderAPI->getDevice());
	//m_resourceManager->loadDefaults();
	//loadShaders();

	m_drawCommander = new ADrawCommander();

	m_sceneManager = new ASceneManager();
	m_sceneManager->init();
	//m_sceneManager->loadScene();

	m_renderAPI->getRenderer().setCamera(m_sceneManager->getCurrentCamera());

	while (!m_quit)
	{
		update();

		render();

		g_input.registerFrameCompletion();

		// api specific event handling, input etc
		m_renderAPI->handleEvents();
	}
}

void AGN::AAplication::cleanup()
{
	delete m_sceneManager;
	delete m_drawCommander;
	delete m_resourceManager;
}

void AGN::AAplication::update()
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

void AGN::AAplication::updateMeshShaderProperties(float a_deltaTime)
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

	m_meshShaderPipeline->setConstantBufferData("LightSettings", &buffer, 48);
}

void AGN::AAplication::loadShaders()
{
	// create shaders
	std::vector<AGN::IAShader*> meshShaders;
	meshShaders.push_back(&m_resourceManager->createShader(g_shader_mesh_vert, EAShaderType::VertexShader));
	meshShaders.push_back(&m_resourceManager->createShader(g_shader_mesh_pix, EAShaderType::PixelShader));
	m_meshShaderPipeline = &m_resourceManager->createShaderPipeline(meshShaders);

	/*
	// TODO: RESTORE after HLSL
	std::vector<AGN::IAShader*> skyboxShaders;
	skyboxShaders.push_back(&m_resourceManager->createShader(g_shader_skybox_vert, EAShaderType::VertexShader));
	skyboxShaders.push_back(&m_resourceManager->createShader(g_shader_skybox_pix, EAShaderType::PixelShader));
	m_skyboxShaderPipeline = &m_resourceManager->createShaderPipeline(skyboxShaders);
	*/
}

void AGN::AAplication::render()
{
	// high level rendering
	createDrawQueue();
	m_drawCommander->sortCommandList();

	// api specific rendering
	m_renderAPI->getRenderer().render(*m_drawCommander);

	// clear render buckets at the end of the frame (after data is uploaded to the GPU)
	m_drawCommander->clearCommands();
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
		AClearBufferData& data = drawCommand.data.clearBufferData;
		data.buffersToClear = (uint32_t)ADrawBufferType::COLOR | (uint32_t)ADrawBufferType::DEPTH;
		data.clearColor = 0x0000FF00;
	}

	/*
	// TODO: Restore after DX11 implementation
	const std::vector<AEntity*> skyboxEntities = m_sceneManager->getSkyboxEntities();
	for (unsigned int i = 0; i < skyboxEntities.size(); i++)
	{
		AEntity& entity = *skyboxEntities[i];

		// calculate model matrix
		mat4 translation = glm::translate(entity.getPosition());
		mat4 rotation = toMat4(entity.getRotation());
		mat4 scaling = scale(entity.getScale());
		mat4 modelMatrix = translation * rotation * scaling;

		for (unsigned int j = 0; j < entity.getMeshCollection().size(); j++)
		{
			IAMesh* mesh = entity.getMeshCollection()[j];
			AMaterial* material = mesh->getMaterial();

			// create sortkey
			uint8_t renderPhase = (uint8_t)RenderPhase::FullscreenViewport;
			uint8_t layer = (uint8_t)RenderLayer::Skybox;
			uint8_t translucencyType = 0;				// TODO:
			uint8_t cmd = 0;							// TODO: ?
			uint16_t shaderPipelineId = m_skyboxShaderPipeline->getAId();
			uint32_t meshId = mesh->getAId();
			uint16_t materialId = material->getAId();
			uint32_t depth = 0;							// TODO:
			uint64_t sortkey = ADrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);

			ADrawCommand& drawCommand = m_drawCommander->addDrawCommand(EADrawCommandType::DrawEntity, sortkey);
			ADrawEntityData& data = drawCommand.data.entityData;
			data.shaderPipeline = m_skyboxShaderPipeline;
			data.material = material;
			data.mesh = mesh;

			// store matrix as float array (for the union data type to work)
			memcpy(&data.modelMatrixArray[0], glm::value_ptr(modelMatrix), sizeof(data.modelMatrixArray));
		}
	}
	*/

	// TODO: make these static draw commands
	// Clear Z Buffer after skybox
	{
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
		AClearBufferData& data = drawCommand.data.clearBufferData;
		data.buffersToClear = (uint32_t)ADrawBufferType::DEPTH;
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
	const std::vector<AEntity*> entities = m_sceneManager->getEntities();
	for (unsigned int i = 0, depthVectorIndex = 0; i < entities.size(); i++)
	{
		AEntity& entity = *entities[i];
		
		// calculate model matrix
		mat4 translation = glm::translate(entity.getPosition());
		mat4 rotation = toMat4(entity.getRotation());
		mat4 scaling = scale(entity.getScale());
		mat4 modelMatrix = translation * rotation * scaling;

		for (unsigned int j = 0; j < entity.getMeshCollection().size(); j++, depthVectorIndex++)
		{
			IAMesh* mesh = entity.getMeshCollection()[j];
			AMaterial* material = mesh->getMaterial();
			 
			// create sortkey
			uint8_t renderPhase = (uint8_t)RenderPhase::FullscreenViewport;
			uint8_t layer = (uint8_t)RenderLayer::Entities;
			uint8_t translucencyType = 0;				// TODO:
			uint8_t cmd = 0;							// TODO: ?
			uint16_t shaderPipelineId = m_meshShaderPipeline->getAId();
			uint32_t meshId = mesh->getAId();
			uint16_t materialId = material->getAId();
			uint32_t depth = 0;//(depthVector[depthVectorIndex] / (maxDepth+1)) * maxDepthEntries; // add sorted depth
			uint64_t sortkey = ADrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, shaderPipelineId, meshId, materialId, depth);
			
			ADrawCommand& drawCommand = m_drawCommander->addDrawCommand(EADrawCommandType::DrawEntity, sortkey);
			ADrawEntityData& data = drawCommand.data.entityData;
			data.shaderPipeline = m_meshShaderPipeline;
			data.material = material;
			data.mesh = mesh;

			// store matrix as float array (for the union data type to work)
			memcpy(&data.modelMatrixArray[0], glm::value_ptr(modelMatrix), sizeof(data.modelMatrixArray));
		}
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