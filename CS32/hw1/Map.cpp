#include "Map.h"
#include <string>

Map::Map() {         // Create an empty map (i.e., one with no key/value pairs)
	m_size = 0;
}

bool Map::empty() const{	// Return true if the map is empty, otherwise false.
	if (size() == 0)
		return true;
	return false;
}

int Map::size() const{		// Return the number of key/value pairs in the map.
	return m_size; //calculates the number of elements in the array
}

bool Map::insert(const KeyType& key, const ValueType& value) {
	if (size() == DEFAULT_MAX_ITEMS)
		return false;
	for (int i = 0; i < size(); i++)
		if (pairs[i].Key == key)
			return false;
	Pair insert{key, value};
	pairs[size()] = insert;
	m_size++;
	return true;
}
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).

bool Map::update(const KeyType& key, const ValueType& value) {
	for (int i = 0; i < size(); i++)
		if (pairs[i].Key == key) {
			pairs[i].Value = value;
			return true;
		}
	return false;
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	if (update(key, value))
		return true;
	if (insert(key, value))
		return true;
	return false;
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value that it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).

bool Map::erase(const KeyType& key) {
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
		if (pairs[i].Key == key) {
			for (int j = i; j < (size() - 1); j++)
				pairs[j] = pairs[j + 1];
			pairs[DEFAULT_MAX_ITEMS - 1].Key = '\0';		//I don't know how to delete an element from an array
			pairs[DEFAULT_MAX_ITEMS - 1].Value = '\0';
			m_size--;
			return true;
		}
	return false;
}

// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.

bool Map::contains(const KeyType& key) const{
	for (int i = 0; i < size(); i++)
		if (pairs[i].Key == key)
			return true;
	return false;
}
// Return true if key is equal to a key currently in the map, otherwise
// false.

bool Map::get(const KeyType& key, ValueType& value) const {
	for (int i = 0; i < size(); i++)
		if (pairs[i].Key == key) {
			value = pairs[i].Value;
			return true;
		}
	return false;
}
// If key is equal to a key currently in the map, set value to the
// value in the map which that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.

bool Map::get(int i, KeyType& key, ValueType& value) const{
	if (i < 0 || i >= size())
		return false;
	key = pairs[i].Key;
	value = pairs[i].Value;
	return true;
}
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)

void Map::swap(Map& other) {
	Pair temp;
	int tempsize = m_size;
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
		temp = pairs[i];
		pairs[i] = other.pairs[i];
		other.pairs[i] = temp;
	}
	m_size = other.m_size;
	other.m_size = tempsize;
}
// Exchange the contents of this map with the other one.