/****************************************************************************/
/* Shared header on top of every cpp (for data types & cross platform libs) */
/****************************************************************************/

#pragma once

// Standard lib
#include <iostream>
#include <string>
#include <ios>
#include <stdint.h>
#include <vector>
#include <cassert>

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
#include "iainput.hpp"
#include "asharedapi.hpp" // different per API 