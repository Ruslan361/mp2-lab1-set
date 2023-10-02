#include <iostream>
#include "tset.h"

int main() {
	const int size = 4;
	const int k = 3;
	TSet set(size), updatedSet(size);
	set.InsElem(0);
	set.InsElem(2);
	updatedSet = set + k;
	std::cout << updatedSet.IsMember(k);
}