// Map.cpp

#include "Map.h"

Map::Map() : m_size(0)	//initialize with size of 0	
{
	//intialize dummy node with m_head
	m_head = new Pair;
	m_head->m_next = m_head;
	m_head->m_prev = m_head;
}

//copy
Map::Map(const Map& other) : m_size(other.m_size)
{
	//still have to initialize dummy node
	m_head = new Pair;
	m_head->m_next = m_head;
	m_head->m_prev = m_head;

	//creating new Map with same number of nodes as other map
	Pair* tempp = m_head;
	for (int i = 0; i < m_size; i++) {	
		Pair* node = new Pair;
		node->m_prev = tempp;
		node->m_next = tempp->m_next;
		tempp->m_next->m_prev = node;
		tempp->m_next = node;
		tempp = node;
	}

	//filling empty nodes with information from other map
	tempp = m_head->m_next;
	for (Pair* p = other.m_head->m_next; p != other.m_head; p = p->m_next) {
		tempp->m_key = p->m_key;
		tempp->m_value = p->m_value;
		tempp = tempp->m_next;
	}
}

//Delete
Map::~Map() 
{
	//while loop iterates through nodes, reassigning m_next and m_prev pointers before deleting the node;
	while (m_head->m_next!=m_head) {
		Pair* obj = m_head->m_next;
		obj->m_next->m_prev = m_head;
		m_head->m_next = obj->m_next;
		delete obj;
	}
	//finishes by deleting the last m_head pointer
	delete m_head;
}

Map& Map::operator=(const Map& rhs)	//assignment operator
{
	if (this != &rhs)
	{
		Map temp(rhs);
		swap(temp);
	}
	return *this;
}

bool Map::erase(const KeyType& key)
{
	int pos = find(key);
	if (pos == -1)  // not found
		return false;

	// Move last array item to replace the one to be erased
	int j = 0;
	//for loop iterates through the nodes until it reaches the node of the key to delete which coincides with the position
	for (Pair* p = m_head->m_next; p != m_head; p = p->m_next) {
		if (j == pos) {
			p->m_prev->m_next = p->m_next;
			p->m_next->m_prev = p->m_prev;
			delete p;
			break;
		}
		j++;
	}
	m_size--;
	return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	int pos = find(key);
	if (pos == -1)  // not found
		return false;
	int j = 0;

	//iterates through the loop until it finds the position of the desired key and saves the key's value to value
	for (Pair* p = m_head->m_next; p != m_head; p = p->m_next) {
		if (j == pos) {
			value = p->m_value;
			break;
		}
		j++;
	}
	return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i < 0 || i >= m_size)
		return false;

	int j = 0;

	//saves the the key and value of the node at position j
	for (Pair* p = m_head->m_next; p != m_head; p = p->m_next) {
		if (j == i) {
			key = p->m_key;
			value = p->m_value;
			break;
		}
		j++;
	}
	return true;
}

void Map::swap(Map& other)
{
	// swapping the m_head pointers to the dummy nodes and the sizes

	Pair* thead = m_head;
	m_head = other.m_head;
	other.m_head = thead;

	int tsize = m_size;
	m_size = other.m_size;
	other.m_size = tsize;
}

int Map::find(const KeyType& key) const
{
	// Do a linear search through the array.
	int i = 0;

	//iterating through the linked list until it finds the position of the desired key. If it fails, returns -1.
	for (Pair* p = m_head->m_next; p != m_head; p = p->m_next)
		if (p->m_key == key)
			return i;
		else
			i++;
	return -1;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value, bool mayInsert, bool mayUpdate)
{
	int pos = find(key);
	if (pos != -1)  // found
	{
		if (mayUpdate) {
			int i = 0;
			for (Pair* p = m_head->m_next; p != m_head; p = p->m_next) {
				if (i == pos) {
					p->m_value = value;
					return mayUpdate;
				}
				i++;
			}
		}
		else
			return false;
	}
	if (!mayInsert)  // not found, and not allowed to insert
		return false;

	//inserting a new node with key and value
	Pair* nodep = new Pair;
	nodep->m_key = key;
	nodep->m_value = value;
	Pair* temp = m_head->m_prev;
	m_head->m_prev->m_next = nodep;
	m_head->m_prev = nodep;
	
	//setting the new nodes m_next and m_prev pointers
	nodep->m_next = m_head;
	nodep->m_prev = temp;
	
	m_size++;
	return true;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	//ensure that the result map is empty beforehand
	Map empty;
	result.swap(empty);
	
	bool confirmation = true;
	KeyType tempKey;
	ValueType tempValue;

	//makes the result map a copy of m1
	for (int i = 0; i < m1.size(); i++) { 
		m1.get(i, tempKey, tempValue);
		result.insert(tempKey, tempValue);
	}
	KeyType tempKey2;
	ValueType tempValue2;
	//for loop iterates through m2 to see if result shares a Key. If it does check to see they have corresponding values
	for (int j = 0; j < m2.size(); j++) {
		m2.get(j, tempKey2, tempValue2);	 
		if (result.contains(tempKey2)) {
			result.get(tempKey2, tempValue);
			//if their values are different, erase the pair from result
			if (tempValue != tempValue2) {	
				result.erase(tempKey2);
				confirmation = false;
			}
			else
				continue;	//if their values are the same, they have a matching pair so do nothing.
		}
		else
			result.insert(tempKey2, tempValue2);	//if they don't have mathing keys, we can just insert the pair from m2 in to result.
	}
	return confirmation;
}

void reassign(const Map& m, Map& result)
//This function works by taking two pairs at a time out of m and inserting them into result but with their values swapped
{
	//ensure that the result map is empty beforehand
	Map mcopy=m;
	Map empty;
	result.swap(empty);

	KeyType tempKey;
	ValueType tempValue;
	KeyType tempKey2;
	ValueType tempValue2;
	KeyType tempKey3;
	ValueType tempValue3;

	//Our method of taking two out at time won't work if m has an odd size, cause we'll then be left with a single unchanged pair
	//so we need to check if m's size is odd in order to scramble three pairs in the beginning instead of just two.
	bool odd = false;
	if (mcopy.size() == 0)
		return;
	if (mcopy.size() == 1) {
		mcopy.get(0, tempKey, tempValue);
		result.insert(tempKey, tempValue);
		return;
	}
	if (mcopy.size() % 2 == 1)
		odd = true;
	for (int i = 0; i < mcopy.size(); i++) {
		if (odd) {
			mcopy.get(i, tempKey, tempValue);
			i++;
			mcopy.get(i, tempKey2, tempValue2);
			i++;
			mcopy.get(i, tempKey3, tempValue3);
			result.insert(tempKey, tempValue3);
			result.insert(tempKey2, tempValue);
			result.insert(tempKey3, tempValue2);
			odd = false;
		}
	//after scrambling the first three pairs of m in the case the size is odd, the for loop will continue iterating
	//scrambling just two pairs at a time.
		mcopy.get(i, tempKey, tempValue);
		i++;
		mcopy.get(i, tempKey2, tempValue2);
		result.insert(tempKey, tempValue2);
		result.insert(tempKey2, tempValue);
	}
}