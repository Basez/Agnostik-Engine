/************************************************************************/
/* Shared header on top of every cpp (for data types                    */
/************************************************************************/

#pragma once

#include <iostream>

// GLM
#define GLM_FORCE_RADIANS // force use of radians everywhere
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <stdint.h>