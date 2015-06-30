#pragma once

#include "iawindow.hpp"

namespace AGN
{
	class AWindowGL : public IAWindow
	{
	public:
		AWindowGL();

		void setTitle(const char* a_title) override;
		void showMessageBox(const char* a_title, const char* a_message) override;

		glm::ivec2 getDimentions() override { return m_dimentions; }

	private:
		glm::ivec2 m_dimentions;

	};
}