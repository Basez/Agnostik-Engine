#pragma once

#include <glm/glm.hpp>

namespace AGN
{
	class IWindow
	{
	public:
		virtual void setTitle(const char* a_title) = 0;
		virtual void showMessageBox(const char* a_title, const char* a_message) = 0;
		virtual void warpCursor(glm::ivec2 a_screenPosition) = 0;
		virtual void showCursor(bool a_shown) = 0;
		virtual void updateWindowState() = 0;
		virtual bool isDirty() = 0;

		virtual glm::ivec2 getDimentions() = 0;

		virtual ~IWindow() {}

	
	};
}