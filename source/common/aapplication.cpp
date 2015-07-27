#include "asharedh.hpp"
#include "aaplication.hpp"
#include "iarender_api.hpp"
#include "iawindow.hpp"
#include "ascenemanager.hpp"
#include "aconfigmanager.hpp"

using namespace AGN;

AAplication appTemp = AAplication();
AAplication& g_application = appTemp;

void AAplication::run(class IARenderAPI* a_renderAPI)
{
	m_renderAPI = a_renderAPI;
	m_quit = false;

	int* testIntPointer = nullptr;

	g_log.debug("pointer size: %i", sizeof(testIntPointer));
	g_log.info("info test");
	g_log.debug("debug test");
	g_log.warning("warning test");
	g_log.error("error test");

	m_renderAPI->init();
	
	m_sceneManager = new ASceneManager();
	m_sceneManager->init();

	m_sceneManager->addNode(); // Add model somehow?

	while (!m_quit)
	{
		update();
		fetchRender();

		m_renderAPI->renderDrawQueue();

		m_renderAPI->getWindow().getDimentions();
	}
}

void AAplication::cleanup()
{
	m_renderAPI = nullptr;
}

void AAplication::update()
{

}

void AAplication::fetchRender()
{
	// fetch all draw calls 
}


IARenderAPI& AAplication::getRenderAPI()
{
	return dynamic_cast<IARenderAPI&>(*m_renderAPI);
}
