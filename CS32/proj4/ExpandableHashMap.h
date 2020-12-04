#ifndef HASH_MAP
#define HASH_MAP

#include <vector>
#include <queue>
#include <functional>
using namespace std;

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	struct Node {
		KeyType key;
		ValueType value;
		Node* next;
	};

	double m_maxLoad;
	
	//keeps track of how many nodes there are
	int m_nNodes;
	vector<Node*> m_buckets;
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor) {
	m_maxLoad = maximumLoadFactor;
	m_nNodes = 0;
	
	//hashmap is set to a size of 8 and its filled with null pointers
	m_buckets.resize(8, nullptr);
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap() {
	//deletes the bucket objects
	for (size_t i = 0; i < m_buckets.size(); i++) {
		while (m_buckets[i] != nullptr) {
			Node* temp = m_buckets[i];
			m_buckets[i] = m_buckets[i]->next;
			delete temp;
		}
	}
	m_buckets.clear();
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset() {
	//deletes the bucket objects
	for (size_t i = 0; i < m_buckets.size(); i++) {
		while (m_buckets[i] != nullptr) {
			Node* temp = m_buckets[i];
			m_buckets[i] = m_buckets[i]->next;
			delete temp;
		}
	}

	//resizes the vector
	m_buckets.resize(8);

	//overwrites the previous dangling pointers
	for (int i = 0; i < 8; i++)
		m_buckets[i] = nullptr;
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const {
	return m_buckets.size();
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
	unsigned int hasher(const KeyType & k);
	Node* location = m_buckets[hasher(key) % m_buckets.size()];

	//iterate through the nodes in the linked list until key is found if there is a list already; update the key if found
	while (location != nullptr ) {
		if (location->key == key) {
			location->value == value;
			return;
		}
		location = location->next;
	}

	//if there is no node or the key is not in the list, put in a new node with key and value and return;   
	if (location == nullptr) {
		double load = (m_nNodes + 1) / (m_buckets.size()+1);

		//IF WHEN LOAD EXCEEDS DESIGNATED MAX LOAD;
		if (load > m_maxLoad) {
			int size = m_buckets.size();
			size *= 2;

			//store all pointers to nodes into a queue
			//time complexity of O(N) because it visits each node once.
			queue<Node*> storage;
			for (size_t i = 0; i < m_buckets.size(); i++) {
				if (m_buckets[i] != nullptr) {
					storage.push(m_buckets[i]);
					Node* nextNode = m_buckets[i]->next;
					while (nextNode != nullptr) {
						storage.push(nextNode);
						nextNode = nextNode->next;
					}
				}
			}

			//resizing and resetting
			m_buckets.resize(size, nullptr);
			for (size_t i = 0; i < m_buckets.size(); i++)
				m_buckets[i] = nullptr;

			//rehash and store pointers to nodes from queue into new hash map
			//time complexity of O(N) because it visits each node once.
			while (!storage.empty()) {
				Node* tempNode = storage.front();
				//nodes are inserted onto the top of the linked list at each of the vector's spaces
				tempNode->next = m_buckets[hasher(tempNode->key) % m_buckets.size()];
				storage.pop();
				m_buckets[hasher(tempNode->key) % m_buckets.size()] = tempNode;
			}
		}

		Node* newNode = new Node;
		newNode->key = key;
		newNode->value = value;

		//the new Node is inserted onto the top of the linked list
		newNode->next = m_buckets[hasher(key) % m_buckets.size()];
		m_buckets[hasher(key) % m_buckets.size()] = newNode;
		m_nNodes++;
		return;
	}
	return;
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const {
	unsigned int hasher(const KeyType & k);
	Node* location = m_buckets[hasher(key) % m_buckets.size()];
	
	//if there is an established linked list, iterate through it to find the key
	while (location != nullptr) {
		if (location->key == key) {
			return &location->value;
		}
		location = location->next;
	}

	//will return nullptr if key is not found;
	return nullptr;
}

#endif