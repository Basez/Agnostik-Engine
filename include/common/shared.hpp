/****************************************************************************/
/* Shared header on top of every cpp (for data types & cross platform libs) */
/****************************************************************************/

#pragma once

// uncomment to enable mmgr memory leak detection
// TODO: find a way to not compile the obj file --> disabling this define still compiles the obj and leaks are still detected somehow? :/
#define AGN_ENABLE_MEMORYLEAK_DETECTION

// stl
#include <iostream>
#include <string.h>
#include <ios>
#include <stdint.h>
#include <vector>
#include <cassert>
#include <sstream>

// GLM
#define GLM_FORCE_RADIANS // force use of radians everywhere
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Agnostik shared classes
#include "i_logger.hpp"

// TODO: move to its own class
namespace AGN
{
	static size_t cStringCopy(char *a_destination, char const *a_source, size_t a_sizeBytes)
	{
		return snprintf(a_destination, a_sizeBytes, "%s", a_source);
	}

	static size_t cStringConcatenate(char *a_destination, char const *a_source, size_t a_sizeBytes)
	{
		return snprintf(a_destination, a_sizeBytes, "%s%s", a_destination, a_source);
	}
};

// TODO: move to its own class
// Safely release a COM object.
template<typename T>
inline void safeRelease(T& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}