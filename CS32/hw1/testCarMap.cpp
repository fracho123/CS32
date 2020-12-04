#include "CarMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	CarMap m;
	CarMap n;
	string l = "";
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
		l += "x";
		n.addCar(l);
	}
	assert(!n.addCar("sakfhsasfas"));
	assert(m.addCar("sakfhsasfas"));
	assert(!m.addCar("sakfhsasfas"));
	assert(m.miles("sakfhsasfas") == 0);
	assert(m.miles("sakfhs") == -1);
	assert(!n.drive("sakfhsasfas", 12313));
	assert(m.drive("sakfhsasfas", 12313));
	assert(m.miles("sakfhsasfas")==12313);
	assert(m.drive("sakfhsasfas", 123));
	assert(m.miles("sakfhsasfas") == 12436);
	assert(n.fleetSize() == DEFAULT_MAX_ITEMS);
	assert(m.fleetSize() == 1);
	assert(m.addCar("sakfhss"));
	assert(m.drive("sakfhss", 3423));
	assert(m.addCar("sakfhsas"));
	assert(m.drive("sakfhsas", 324324));
	assert(m.addCar("sakfhs"));
	assert(m.drive("sakfhs", 342325));
	assert(m.addCar("sakfhsasfassafjkahdas"));
	assert(m.drive("sakfhsasfassafjkahdas",2389247));
	assert(m.addCar("sakfhsasfasss"));
	m.print();

	
	cout << "Passed all tests" << endl;
}