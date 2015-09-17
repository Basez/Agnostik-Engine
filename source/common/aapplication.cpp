#include "asharedh.hpp"
#include "aaplication.hpp"

#include "iarender_api.hpp"
#include "iawindow.hpp"
#include "iarenderer.hpp"
#include "iamesh.hpp"
#include "iamaterial.hpp"

#include "aentity.hpp"
#include "ascenemanager.hpp"
#include "aconfigmanager.hpp"
#include "aresourcemanager.hpp"
#include "adrawcommander.hpp"
#include "adrawcommand.hpp"

AGN::AAplication appTemp = AGN::AAplication();
AGN::AAplication& g_application = appTemp;

void AGN::AAplication::run(class IARenderAPI* a_renderAPI)
{
	m_renderAPI = a_renderAPI;
	m_quit = false;

	m_renderAPI->init();
	
	m_resourceManager = new AResourceManager();
	m_resourceManager->init();

	m_drawCommander = new ADrawCommander();

	m_sceneManager = new ASceneManager();
	m_sceneManager->init();
	m_sceneManager->loadTestScene01();

	while (!m_quit)
	{
		update();
		createDrawQueue();
		m_drawCommander->sortCommandList();
		m_renderAPI->getRenderer().render(*m_drawCommander);

		// clear render buckets at the end of the frame (after data is uploaded to the GPU)
		m_drawCommander->clearCommands();
	}

}

void AGN::AAplication::cleanup()
{
	m_renderAPI = nullptr;
}

void AGN::AAplication::update()
{
	// logic
}

void AGN::AAplication::createDrawQueue()
{
	const std::vector<AEntity*> entities = m_sceneManager->getEntities();

	// fill commander with entity draw commands
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		// get sortkey
		uint8_t renderPhase = 0;		// TODO:
		uint8_t layer = 0;				// TODO:
		uint8_t translucencyType = 0;	// TODO:
		uint8_t cmd = 0;				// TODO: ?
		uint16_t meshId = entities[i]->getMesh()->getId();
		uint16_t materialId = entities[i]->getMaterial()->getId();
		uint32_t depth = 0;				// TODO:

		uint64_t sortkey = ADrawCommander::getSortKey(renderPhase, layer, translucencyType, cmd, meshId, materialId, depth);

		ADrawCommand& drawCommand = m_drawCommander->addDrawCommand(EADrawCommandType::DrawEntity, sortkey);
		ADrawEntityData& data = drawCommand.data.entityData;
		data.mesh = entities[i]->getMesh();
		data.shaderProgram = entities[i]->getMaterial();
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