#include "asharedh.hpp"
#include "aaplication.hpp"
#include "iarender_api.hpp"
#include "iawindow.hpp"

using namespace AGN;

AAplication appTemp = AAplication();
AAplication& g_application = appTemp;

void AAplication::run(class IARenderAPI* a_renderAPI)
{
	m_renderAPI = a_renderAPI;
	m_quit = false;

	int* testIntPointer = nullptr;
	printf("pointer size: %i \n", sizeof(testIntPointer));

	m_renderAPI->init();

	while (!m_quit)
	{
		update();
		render();

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

void AAplication::render()
{

}
