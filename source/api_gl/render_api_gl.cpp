#include "shared.hpp"
#include "render_api_gl.hpp"
#include "window_gl.hpp"
#include "device_gl.hpp"
#include "renderer_gl.hpp"
#include "config_manager.hpp"
#include "input_gl.hpp"
#include "application.hpp"
#include "gui_gl.hpp"

#include <GL/glew.h>
#include <SDL/SDL.h>

AGN::RenderAPIGL::RenderAPIGL()
	: m_initialized(false)
	, m_window(nullptr)
	, m_device(nullptr)
	, m_renderer(nullptr)
	, m_gui(nullptr)
{
	m_device = new DeviceGL();
	m_renderer = new RendererGL();
	m_gui = new GUIGL();
}

bool AGN::RenderAPIGL::init()
{
	if (!initOpenGL() || !initGlew())
	{
		// issue occured during initializing OpenGL context
		return false;
	}

	// Log info about initialized openGL
	g_log.info("Initialized OpenGL version : %s", glGetString(GL_VERSION));
	g_log.info("vendor: %s", glGetString(GL_VENDOR));
	g_log.info("Renderer: %s", glGetString(GL_RENDERER));

	// log num extensions
	int32_t numExtensions = -1;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	g_log.info("GL extensions count: %i", numExtensions);

	m_device->init();
	m_renderer->init();

	enableVSync(g_configManager.getConfigPropertyAsBool("vsync"));

	m_gui->init(m_window->getSDLWindow());

	m_initialized = true;

	AGN::RenderAPIGL::getOpenGLErrors();

	return true;
}

bool AGN::RenderAPIGL::initOpenGL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		auto error = SDL_GetError();
		g_log.error("error: %s", error);
		return false;
	}

	// TODO: implement fonts for OpenGL
	//TTF_Init();

	// Create an OpenGL 3.3 core forward compatible context.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);	// allows deprecated stuff
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	
	//SDL_SetRelativeMouseMode(SDL_TRUE);  // hide mouse, FPS style
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// anti aliasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

	// create window
	glm::ivec2 resolution;
	resolution.x = g_configManager.getConfigPropertyAsInt32("start_resolution_x");
	resolution.y = g_configManager.getConfigPropertyAsInt32("start_resolution_y");
	m_window = new WindowGL(resolution);

	SDL_GL_CreateContext(m_window->getSDLWindow());
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	AGN::RenderAPIGL::getOpenGLErrors();

	return true;
}

bool AGN::RenderAPIGL::initGlew()
{
	glewExperimental = GL_TRUE;

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		g_log.error("Error initializing GLEW");
		return false;
	}

	// Check for 3.3 support.
	// I've specified that a 3.3 forward-compatible context should be created.
	// so this parameter check should always pass if our context creation passed.
	// If we need access to deprecated features of OpenGL, we should check
	// the state of the GL_ARB_compatibility extension.
	if (!GLEW_VERSION_3_3)
	{
		g_log.error("MISSING 3.3 required version support not present.");

		const char* errorHeader = "Unsupported OpenGL Version";
		char errorMessageC[1024] = "Cannot initialize OpenGL 3.3, this device is running: OpenGL ";
		AGN::cStringConcatenate(errorMessageC, reinterpret_cast<const char*>(glGetString(GL_VERSION)), sizeof(errorMessageC));
		m_window->showMessageBox(errorHeader, errorMessageC);
		return false;
	}

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.warning("An OpenGL error occurred during GLEW initialization: %X It is safe to ignore this issue", errorType);
	}

	return true;
}

AGN::IWindow& AGN::RenderAPIGL::getWindow()
{
	return dynamic_cast<IWindow&>(*m_window);
}

AGN::IDevice& AGN::RenderAPIGL::getDevice()
{
	return dynamic_cast<IDevice&>(*m_device);
}

AGN::IRenderer& AGN::RenderAPIGL::getRenderer()
{
	return dynamic_cast<IRenderer&>(*m_renderer);
}

AGN::IGUI& AGN::RenderAPIGL::getGUI()
{
	return dynamic_cast<AGN::IGUI&>(*m_gui);
}

void AGN::RenderAPIGL::logAvailableGLExtensions()
{
	if (!m_initialized)
	{
		g_log.error("Please Initialize OpenGL first");
		return;
	}

	int32_t numExtensions = -1;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	g_log.debug("Number of GL extensions available: %i", numExtensions);
	for (int32_t i = 1; i < numExtensions; i++)
	{
		g_log.debug("Extension Enabled: %s", glGetStringi(GL_EXTENSIONS, i));
	}
}

void AGN::RenderAPIGL::getOpenGLErrors()
{
	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An OpenGL error occurred: %X", errorType);
	}

#ifdef AGN_DEBUG
		//assert(false);
#endif
}

void AGN::RenderAPIGL::enableVSync(bool a_value)
{
	m_vSync = a_value;
	if (a_value)
	{
		// TODO: for working vsync on linux, apparently I should use GLFW instead of SDL
		// Link: https://www.gamedev.net/topic/482099-vsync-with-linux-sdlgl/
		SDL_GL_SetSwapInterval(1);
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}
}

void AGN::RenderAPIGL::handleEvents(bool& a_doQuit)
{
	a_doQuit = false;
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent))
	{
		m_gui->processEvent(&sdlEvent);

		switch (sdlEvent.type)
		{
		case SDL_QUIT:	
			a_doQuit = true;
			break;

		case SDL_KEYDOWN:
			g_input.registerHold(InputGL::getAGNScanCode(sdlEvent.key.keysym.scancode), true);
			//g_log.debug("GL scancode: %i, agnscancode: %u", (uint16_t)event.key.keysym.scancode, (uint16_t)AInputGL::getAGNScanCode(event.key.keysym.scancode));
			break;

		case SDL_KEYUP:
			g_input.registerHold(InputGL::getAGNScanCode(sdlEvent.key.keysym.scancode), false);
			break;

		case SDL_MOUSEBUTTONDOWN:
			g_input.registerMouseHold(InputGL::getAGNMouse(sdlEvent.button.button), true);
			break;

		case SDL_MOUSEBUTTONUP:
			g_input.registerMouseHold(InputGL::getAGNMouse(sdlEvent.button.button), false);
			g_input.registerMouseClick(InputGL::getAGNMouse(sdlEvent.button.button));
			break;

		case SDL_MOUSEMOTION:
			g_input.registerMouseMotion(sdlEvent.motion.x, sdlEvent.motion.y);
			//g_cameraController->onMouseMotion(event.motion.x, event.motion.y);
			break;

		case SDL_MOUSEWHEEL:
			g_input.registerMouseScroll((int)sdlEvent.wheel.y);
			break;

		case SDL_WINDOWEVENT:
			m_window->onWindowEvent(sdlEvent);

			if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				int windowSizeX = sdlEvent.window.data1;
				int windowSizeY = sdlEvent.window.data2;
				g_log.info("window resized to %i/%i", windowSizeX, windowSizeY);
			}
			break;
		}
	}
}
