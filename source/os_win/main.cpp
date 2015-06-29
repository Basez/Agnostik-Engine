#include <iostream>

int main(int argc, int* argv)
{
	int* testIntPointer = new int();
	*testIntPointer = 5;
	printf("pointer size: %i \n", sizeof(testIntPointer));
	delete testIntPointer;
	
	while(1) continue;
	return 0;
}