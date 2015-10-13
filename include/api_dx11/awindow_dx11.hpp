#pragma once

#include "iawindow.hpp"

namespace AGN
{
	class AWindowDX11 : public IAWindow
	{
	public:
		AWindowDX11(glm::ivec2 a_dimentions);

		void setTitle(const char* a_title) override;
		void showMessageBox(const char* a_title, const char* a_message) override;
		void warpCursor(glm::ivec2 a_screenPosition) override;
		void showCursor(bool a_shown) override;

		glm::ivec2 getDimentions() override { return glm::ivec2(); } // TODO:

	private:
	};
}