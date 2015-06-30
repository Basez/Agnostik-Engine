#include "asharedh.hpp"
#include "aaplication.hpp"


AGN::AAplication::AAplication()
{
	int* testIntPointer = new int();
	printf("pointer size: %i \n", sizeof(testIntPointer));
	delete testIntPointer;
}

AGN::AAplication::~AAplication()
{

}

void AGN::AAplication::init()
{
	printf("AAplication::init()");

	// init scenegraph?
}