#include "asharedh.hpp"
#include "arender_api_gl.hpp"
#include "awindow_gl.hpp"
#include "adevice_gl.hpp"
#include "arenderer_gl.hpp"
#include "aconfigmanager.hpp"

// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


AGN::ARenderAPIGL::ARenderAPIGL()
	: m_window(nullptr)
	, m_device(nullptr)
	, m_renderer(nullptr)
	, m_initialized(false)
{
}

void AGN::ARenderAPIGL::init()
{
	initOpenGL();
	initGlew();

	// Log info about initialized openGL
	g_log.info("Initialized OpenGL version : %s", glGetString(GL_VERSION));
	g_log.info("vendor: %s", glGetString(GL_VENDOR));
	g_log.info("Renderer: %s", glGetString(GL_RENDERER));

	// log num extensions
	GLint numExtensions = -1;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	g_log.info("GL extensions count: %i", numExtensions);

	// create device; class that initializes resources
	m_device = new ADeviceGL();
	m_device->init();

	//  create renderer (class that parses the renderqueue and renders them)
	m_renderer = new ARendererGL();
	m_renderer->init();

	m_initialized = true;

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An OpenGL error occurred during Initialization: %s", AConversionUtils::getAsHexString(errorType).c_str());
	}
}

void AGN::ARenderAPIGL::submitDrawCall()
{

}

void AGN::ARenderAPIGL::renderDrawQueue()
{

}

void AGN::ARenderAPIGL::initOpenGL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		auto error = SDL_GetError();
		g_log.error("error: %s", error);
	}

	// TODO: implement TTF 
	//TTF_Init();

	// Create an OpenGL 3.3 core forward compatible context.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);	// allows deprecated stuff
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	
	//SDL_SetRelativeMouseMode(SDL_TRUE);  // hide mouse, FPS style
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// anti aliasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

	// create window
	glm::ivec2 resolution;
	resolution.x = g_configManager.getConfigPropertyAsInt32("start_resolution_x");
	resolution.y = g_configManager.getConfigPropertyAsInt32("start_resolution_y");
	m_window = new AWindowGL(resolution);

	SDL_GL_CreateContext(m_window->getSDLWindow());
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An OpenGL error occurred during Initialization: %s", AConversionUtils::getAsHexString(errorType).c_str());
	}
}

void AGN::ARenderAPIGL::initGlew()
{
	glewExperimental = GL_TRUE;

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		g_log.error("Error initializing GLEW");
		return;
	}

	// Check for 3.3 support.
	// I've specified that a 3.3 forward-compatible context should be created.
	// so this parameter check should always pass if our context creation passed.
	// If we need access to deprecated features of OpenGL, we should check
	// the state of the GL_ARB_compatibility extension.
	if (!GLEW_VERSION_3_3)
	{
		g_log.error("MISSING 3.3 required version support not present.");
		return;
	}

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.warning("An OpenGL error occurred during GLEW initialization: %s It is safe to ignore this issue", AConversionUtils::getAsHexString(errorType).c_str());
	}
}

AGN::IAWindow& AGN::ARenderAPIGL::getWindow()
{
	return dynamic_cast<IAWindow&>(*m_window);
}

AGN::IADevice& AGN::ARenderAPIGL::getDevice()
{
	return dynamic_cast<IADevice&>(*m_device);
}

AGN::IARenderer& AGN::ARenderAPIGL::getRenderer()
{
	return dynamic_cast<IARenderer&>(*m_renderer);
}

void AGN::ARenderAPIGL::logAvailableGLExtensions()
{
	if (!m_initialized)
	{
		g_log.error("Please Initialize OpenGL first");
		return;
	}

	GLint numExtensions = -1;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	g_log.debug("Number of GL extensions available: %i", numExtensions);
	for (GLint i = 1; i < numExtensions; i++)
	{
		g_log.debug("Extension Enabled: %s", glGetStringi(GL_EXTENSIONS, i));
	}
}