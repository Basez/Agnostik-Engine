/****************************************************************************/
/* Shared header on top of every cpp (for data types & cross platform libs) */
/****************************************************************************/

#pragma once

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
}