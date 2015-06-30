#pragma once

#include "iarender_api.hpp"

namespace AGN
{
	class ARenderAPIGL : public IARenderAPI
	{
	public:
		ARenderAPIGL();

		void init() override;
		void submitDrawCall() override;
		void renderDrawQueue() override;
		virtual class IAWindow& getWindow() override;

	private:
		void initOpenGL();
		void initGlew();

		class AWindowGL* m_window;
	};
}
