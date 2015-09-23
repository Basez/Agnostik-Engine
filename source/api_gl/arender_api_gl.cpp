#include "asharedh.hpp"
#include "arender_api_gl.hpp"
#include "awindow_gl.hpp"
#include "adevice_gl.hpp"
#include "arenderer_gl.hpp"
#include "aconfigmanager.hpp"
#include "ainput_gl.hpp"
#include "aaplication.hpp"

AGN::ARenderAPIGL::ARenderAPIGL()
	: m_initialized(false)
	, m_window(nullptr)
	, m_device(nullptr)
	, m_renderer(nullptr)
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

	enableVSync(g_configManager.getConfigPropertyAsBool("vsync"));

	m_initialized = true;

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An OpenGL error occurred during Initialization: %s", AConversionUtils::getAsHexString(errorType).c_str());
	}
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
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

void AGN::ARenderAPIGL::enableVSync(bool a_value)
{
	/*
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions = (char*)glGetString(GL_EXTENSIONS);

	if (strstr(extensions, "WGL_EXT_swap_control") == 0)
	{
	Log.error("Extension not found?");
	return;
	}
	else
	{
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)	wglSwapIntervalEXT(a_value);
	}
	#else
	Log.error("Vsync is not supported for current platform!");
	#endif
	*/
	if (a_value)
	{
		SDL_GL_SetSwapInterval(1);
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}
}

void AGN::ARenderAPIGL::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:	
			g_application.quit();
			break;

		case SDL_KEYDOWN:
			g_input.registerHold(AInputGL::getAGNKey(event.key.keysym.scancode), true);
			break;

		case SDL_KEYUP:
			g_input.registerHold(AInputGL::getAGNKey(event.key.keysym.scancode), false);
			break;

		case SDL_MOUSEBUTTONDOWN:
			g_input.registerMouseHold(AInputGL::getAGNMouse(event.button.button), true);
			break;

		case SDL_MOUSEBUTTONUP:
			g_input.registerMouseHold(AInputGL::getAGNMouse(event.button.button), false);
			g_input.registerMouseClick(AInputGL::getAGNMouse(event.button.button));
			break;

		case SDL_MOUSEMOTION:
			g_input.registerMouseMotion(event.motion.x, event.motion.y);
			//g_cameraController->onMouseMotion(event.motion.x, event.motion.y);
			break;

		case SDL_MOUSEWHEEL:
			g_input.registerMouseScroll((int)event.wheel.y);
			break;

		case SDL_WINDOWEVENT:
			m_window->onWindowEvent(event);

			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				// TODO: refactor, this is a weird hack?
				int windowSizeX = event.window.data1;
				int windowSizeY = event.window.data2;

				//g_camera3D->setProjectionRH(60.0f, (float)windowSizeX / (float)windowSizeY, 0.1f, 10000.0f);
				//g_camera3D->setViewport(0, 0, windowSizeX, windowSizeY);
				//g_camera3D->onWindowResize();
				g_log.info("window resized to %i/%i", windowSizeX, windowSizeY);
			}
			break;
		}
	}
}
