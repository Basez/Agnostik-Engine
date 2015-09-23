/****************************************************************************/
/* Shared header, with specically shared API functionality (this one is for OpenGL)
/****************************************************************************/

#pragma once

// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
//#include <SDL/SDL_ttf.h>


// used for openGL's VBO initializations
#define BUFFER_OFFSET(i) ((char*)NULL + (i))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))