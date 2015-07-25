#pragma once

#include <glm/glm.hpp>

namespace AGN
{
	class IAWindow
	{
	public:
		virtual void setTitle(const char* a_title) = 0;
		virtual void showMessageBox(const char* a_title, const char* a_message) = 0;

		virtual glm::ivec2 getDimentions() = 0;

		virtual ~IAWindow() {}
	};
}