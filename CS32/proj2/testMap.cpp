#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Map m;
	assert(m.insert("Fred", 123));
	assert(m.insert("Ethel", 456));
	assert(m.size() == 2);
	ValueType v = 42;
	assert(!m.get("Lucy", v) && v == 42);
	assert(m.get("Fred", v) && v == 123);
	v = 42;
	KeyType x = "Lucy";
	assert(m.get(0, x, v) &&
		((x == "Fred" && v == 123) || (x == "Ethel" && v == 456)));
	KeyType x2 = "Ricky";
	assert(m.get(1, x2, v) &&
		((x2 == "Fred" && v == 123) || (x2 == "Ethel" && v == 456)) &&
		x != x2);
}

int main()
{
	test();
	// maps strings to doubles
	Map m;
	Map n;
	Map o;
	KeyType x = "";
	ValueType y = 123;
	ValueType z;
	assert(m.size() == 0);				//check that m is initialized with a size of 0.
	assert(m.insert("xyz", 9876.5));	//check to see if insert function returns true properly
	assert(!m.empty());					//check that m is not longer empty
	assert(!m.insert("xyz", 96.5));		//check to see if insert will return false if key is already in list
	assert(m.insert("xy", 9876.5));		//check to see if insert will still put in a pair even if values are shared
	assert(m.insert("z", 9876.5));		//extra insert just for further testing
	assert(n.insert("abc", 9876.5));	//extra insert for varied maps
	assert(m.contains("xyz"));			//check to see if insert and contains functions are working properly
	assert(m.contains("xy"));			//check to see if insert and contains functions are working properly
	assert(m.contains("z"));			//check to see if insert and contains functions are working properly
	assert(n.contains("abc"));			//check to see if insert and contains functions are working properly
	assert(!n.contains("xyz"));			//check to see if contain function works by not detecting the key
	assert(o.empty());					//check to see if initialization was done properly. Since nothing was added, o should be empty
	assert(!o.erase("zyx"));			//check that there is nothing to erase
	assert(m.size() == 3);				//check that size() works properly
	m.insert("zasfkjah", 9876.5);
	assert(m.size() == 4);				//check that size() changes with the extra insert
	assert(m.erase("xy"));				//check that erase works
	assert(m.size() == 3);				//and that size() will change accordingly
	assert(m.update("xyz", y));			//check that update will return true as it can change the value to "xyz"
	assert(m.get("xyz", z) && z == y);	//check that the previous update function properly updates the value to "xyz"
	assert(!m.update("kajdf", y));		//check that the update function will return false because there is no matching key in m
	for (int i = 0; i < 240; i++) {
		x += "x";
		o.insert(x, 8697.6);
	}
	assert(o.size() == 240);			//check that the size function maintains its accuracy with a large dataset
	assert(o.insertOrUpdate("x", y));	//check that the insert or update returns true
	assert(o.get("x", z) && z == y);	//check that the insert or update function updated the value for key "x"
	assert(o.insertOrUpdate("zzz", y) && o.size() == 241);	//check that in this case insert or update wil insert the pair
	assert(o.contains("x"));			//check that contain returns true
	assert(o.erase("x"));				//check that erase will return true
	assert(!o.erase("yyy"));			//check that erase will return false since the key won't be found in o
	assert(!o.contains("x"));			//check that erase will indeed change whether or not the key will be found by the contain function
	assert(o.get("xxx", y) && y == 8697.6);	//another check on the get function
	assert(o.get(0, x, y));				//check on the get function
	assert(o.get(1, x, y));				//check on the get function

	//check to see that after the swap function is called, their sizes swap as well
	assert(o.size() == 240);
	assert(m.size() == 3);
	m.swap(o);
	assert(o.size() == 3);
	assert(m.size() == 240);

	assert(o.contains("z") && o.contains("zasfkjah"));				//after swapping m and o, check that o contains elements that were in m
	assert(!o.contains("xxx") && !o.contains("xxxxxxxxxxxxxxx"));	//check that o no longer contains its elements from before the swap
	assert(m.contains("xxx") && m.contains("xxxxxxxxxxxxxxx"));		//check that the elements that were in o are now in m

	assert(combine(m, n, o) && o.size() == 241); // properly combines m and n into o despite o already having elements
	Map t, u;
	ValueType thing, otherThing;
	t.insert("x", 2352);
	t.insert("y", 439867);
	t.insert("z", 8974316);
	reassign(m, u);								
	assert(u.get("xxx", thing) && thing == 8697.6); //check that reassign works
	reassign(t, t);								//check that reassign works even when the parameters are of the same map;
	assert(t.get("x", thing) && thing != 2352); //check that the reassign function successfully scrambled the values of the pairs
	reassign(t, u);								//check that reassign works even when result already has elements in it.
	assert(u.get("x", otherThing) && otherThing != thing);	//check that u has elements from t and that those elements are aslo scrambled
	t.insert("a", 3548);
	Map q;
	Map r;
	Map s;
	assert(combine(q, r, s) && r.size() == 0);  //check that combine works with empty maps
	assert(combine(m, n, m));					//check that it still works even when some of the parameters are of the same map
	assert(combine(m, m, m));					//check that it still works even when all parameters are of the same map
	q.insert("akdfj", 2934786);
	reassign(q, q);
	assert(q.get("akdfj", thing) && thing == 2934786); //check that reassign still work when there is only one element in the map
	

	Map p;
	p.insert("A", 10);
	p.insert("B", 44);
	p.insert("C", 10);
	cout << "Passed all tests" << endl;
}