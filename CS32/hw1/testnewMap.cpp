#include "newMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Map m;  // maps strings to doubles
	Map n;
	Map o;
	KeyType x = "";
	ValueType y = 123;
	m.insert("xyz", 9876.5);
	m.insert("xy", 9876.5);
	m.insert("z", 9876.5);
	n.insert("abc", 9876.5);
	assert(m.contains("xyz"));
	assert(m.contains("xy"));
	assert(m.contains("z"));
	assert(n.contains("abc"));
	assert(!n.contains("xyz"));
	assert(o.empty());
	assert(m.size() == 3);
	m.insert("zasfkjah", 9876.5);
	assert(m.size() == 4);
	assert(m.erase("xy"));
	assert(m.size() == 3);
	assert(m.update("xyz", y));
	assert(!m.update("kajdf", y));
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
		x += "x";
		o.insert(x, 8697.6);
	}
	assert(o.size() == DEFAULT_MAX_ITEMS);
	assert(o.insertOrUpdate("x", y));
	assert(!o.insertOrUpdate("y", y));
	assert(o.contains("x"));
	assert(o.erase("x"));
	assert(!o.erase("y"));
	assert(o.size() == DEFAULT_MAX_ITEMS - 1);
	assert(!o.contains("x"));
	assert(!o.get("y", y));
	assert(o.get("xxx", y) && y == 8697.6);
	assert(o.get(0, x, y));
	assert(o.get(1, x, y));
	assert((o.size() == DEFAULT_MAX_ITEMS - 1) && (m.size() == 3));
	m.swap(o);
	assert((m.size() == DEFAULT_MAX_ITEMS - 1) && (o.size() == 3));
	assert(o.contains("z") && o.contains("zasfkjah"));
	assert(!o.contains("xxx") && !o.contains("xxxxxxxxxxxxxxx"));
	assert(m.contains("xxx") && m.contains("xxxxxxxxxxxxxxx"));
	Map a(1000);   // a can hold at most 1000 key/value pairs
	Map b(5);      // b can hold at most 5 key/value pairs
	Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
	KeyType k[6] = { "askjfh", "aksgj", "kasjd", "aksjdh", "vjhca", "fdklgjb"};
	ValueType v = 928;

	// No failures inserting pairs with 5 distinct keys into b
	for (int n = 0; n < 5; n++)
		assert(b.insert(k[n], v));

	// Failure if we try to insert a pair with a sixth distinct key into b
	assert(!b.insert(k[5], v));

	// When two Maps' contents are swapped, their capacities are swapped
	// as well:
	a.swap(b);
	assert(!a.insert(k[5], v) && b.insert(k[5], v));
	cout << "Passed all tests" << endl;
}