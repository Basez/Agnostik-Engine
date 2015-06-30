#include "asharedh.hpp"
#include "aaplication_gl.hpp"
#include "arenderer_gl.hpp"
#include "awindow_gl.hpp"

#include <iostream>

/*
	protected:
	class IARenderer* m_renderer;
	class IAWindow* m_window;
*/

AGN::AAplicationGL::AAplicationGL()
{
	printf("AAplicationGL()");
}

AGN::AAplicationGL::~AAplicationGL()
{

}

void AGN::AAplicationGL::init()
{
	AAplication::init();

	m_renderer = new ARendererGL();
	m_window = new AWindowGL();

}

void AGN::AAplicationGL::run()
{
	while (1) continue;
}

void AGN::AAplicationGL::cleanup()
{
	// TODO: cleanup GL renderer etc
}


