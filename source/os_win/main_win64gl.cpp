#include "asharedh.hpp"
#include "aaplication.hpp"

#include "arender_api_gl.hpp"

using namespace AGN;

int main(int argc, char* argv[])
{
	IARenderAPI* renderAPI = new ARenderAPIGL();

	g_application.run(renderAPI);

	g_application.cleanup(); 

	delete renderAPI;

	return 0;
}