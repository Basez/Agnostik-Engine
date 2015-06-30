#pragma once

#include "aaplication.hpp"

namespace AGN
{
	class AAplicationGL : public AAplication
	{
	public:
		AAplicationGL();
		~AAplicationGL();
	
		void init() override;
		void run() override;
		void cleanup() override;

	private:
		AAplicationGL(const AAplicationGL& other);
		AAplicationGL& operator=(const AAplicationGL& other);

		
	};
}