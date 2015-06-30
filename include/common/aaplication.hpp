#pragma once

namespace AGN
{
	class AAplication
	{
	public:
		void run(class IARenderAPI* a_renderAPI);
		void cleanup();

	private:
		void update();
		void render();

		class IARenderAPI* m_renderAPI;

		bool m_quit;
	};

}
extern AGN::AAplication& g_application;