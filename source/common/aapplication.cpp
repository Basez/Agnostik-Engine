#include "aaplication.hpp"

#include <iostream>

AGN::AAplication::AAplication()
{
	int* testIntPointer = new int();
	printf("pointer size: %i \n", sizeof(testIntPointer));
	delete testIntPointer;
}

AGN::AAplication::~AAplication()
{

}

