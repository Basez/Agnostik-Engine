#include "asharedh.hpp"
#include "aaplication_gl.hpp"

int main(int argc, char* argv[])
{
	AGN::AAplicationGL application;

	application.init();
	
	application.run();

	application.cleanup();

	return 0;
}