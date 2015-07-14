#include "asharedh.hpp"
#include "arender_api_gl.hpp"
#include "iawindow.hpp"
#include "awindow_gl.hpp"

// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


AGN::ARenderAPIGL::ARenderAPIGL()
	: m_window(nullptr)
{
}

void AGN::ARenderAPIGL::init()
{
	initOpenGL();
	initGlew();
}

void AGN::ARenderAPIGL::submitDrawCall()
{

}

void AGN::ARenderAPIGL::renderDrawQueue()
{

}

AGN::IAWindow& AGN::ARenderAPIGL::getWindow()
{
	return dynamic_cast<IAWindow&>(*m_window);
}

void AGN::ARenderAPIGL::initOpenGL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		auto error = SDL_GetError();
		//Log.error("error: %s \n", error);	// TODO:
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
	m_window = new AWindowGL(glm::ivec2(800, 600));

	SDL_GLContext openGL3Context = SDL_GL_CreateContext(m_window->getSDLWindow());
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// TODO:
	//Log.info("Initialized OpenGL version : %s", glGetString(GL_VERSION));
	//Log.info("vendor: %s", glGetString(GL_VENDOR));
	//Log.info("Extensions: %s ", glGetString(GL_EXTENSIONS));
	//Log.info("Renderer: %s", glGetString(GL_RENDERER));
}

void AGN::ARenderAPIGL::initGlew()
{
	glewExperimental = GL_TRUE;

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		//Log.error("Error initting glew \n"); // TODO:
		return;
	}

	// Check for 3.3 support.
	// I've specified that a 3.3 forward-compatible context should be created.
	// so this parameter check should always pass if our context creation passed.
	// If we need access to deprecated features of OpenGL, we should check
	// the state of the GL_ARB_compatibility extension.
	if (!GLEW_VERSION_3_3)
	{
		//Log.error("MISSING 3.3 required version support not present. \n"); // TODO:
		return;
	}
}

