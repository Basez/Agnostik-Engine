/****************************************************************************/
/* Shared header on top of every cpp (for data types & cross platform libs) */
/****************************************************************************/

#pragma once

#include <iostream>
#include <string>
#include <ios>
#include <stdint.h>

// GLM
#define GLM_FORCE_RADIANS // force use of radians everywhere
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Agnostik shared classes
#include "ialogger.hpp"
#include "aconversionutils.hpp"

// TODO: DOCUMENT, used for openGL's VBO initializations
#define BUFFER_OFFSET(i) ((char*)NULL + (i))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))