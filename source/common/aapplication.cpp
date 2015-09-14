#include "asharedh.hpp"
#include "aaplication.hpp"
#include "iarender_api.hpp"
#include "iawindow.hpp"
#include "ascenemanager.hpp"
#include "aconfigmanager.hpp"
#include "aresourcemanager.hpp"

AGN::AAplication appTemp = AGN::AAplication();
AGN::AAplication& g_application = appTemp;

void AGN::AAplication::run(class IARenderAPI* a_renderAPI)
{
	m_renderAPI = a_renderAPI;
	m_quit = false;

	m_renderAPI->init();
	
	m_resourceManager = new AResourceManager();
	m_resourceManager->init();

	m_sceneManager = new ASceneManager();
	m_sceneManager->init();
	m_sceneManager->loadTestScene01();

	while (!m_quit)
	{
		update();
		createDrawQueue();
		sortDrawQueue();

		m_renderAPI->renderDrawQueue();

		m_renderAPI->getWindow().getDimentions();
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
	// fetch all draw calls 
}

void AGN::AAplication::sortDrawQueue()
{
	// sort draw queue
}

AGN::IARenderAPI& AGN::AAplication::getRenderAPI()
{
	return dynamic_cast<AGN::IARenderAPI&>(*m_renderAPI);
}

AGN::AResourceManager& AGN::AAplication::getResourceManager()
{
	return *m_resourceManager;
}