#pragma once

namespace AGN
{
	class AAplication
	{
	public:
		void run(class IARenderAPI* a_renderAPI);
		void cleanup();

		class IARenderAPI* getRenderAPI() { return m_renderAPI; }

	private:
		void update();
		void fetchRender();

		class IARenderAPI* m_renderAPI;
		bool m_quit;
	};
}
extern AGN::AAplication& g_application;