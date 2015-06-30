#pragma once

namespace AGN
{
	class AAplication
	{
	public:
		AAplication();
		~AAplication();
		
		virtual void init();
		virtual void run() = 0;
		virtual void cleanup() = 0;

	protected:
		class IARenderer* m_renderer;
		class IAWindow* m_window;

	private:
		AAplication(const AAplication& other);
		AAplication& operator=(const AAplication& other);

	};
}