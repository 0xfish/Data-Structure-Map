#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>

typedef std::string KeyType;
typedef double ValueType;




class Map
{
private:
	struct MapNode {
		KeyType m_key;
		ValueType m_value;
		MapNode *m_left;
		MapNode *m_right;
	};

	MapNode *root;
	int counter;

	//Private helper function for contains
	bool containsHelper(MapNode* node, const KeyType& key) const;

	//Private helper function for insert
	bool Radd(MapNode* &node, const KeyType& x, const ValueType& y);

	//Copy Constructor helper function
	MapNode* copyMap(const MapNode* node); 
	
	//Function to destroy the tree recursively; helps the destructor.
	void destroyTree(MapNode *node); 

	//erase helper functions
	bool eraseR(MapNode* v, MapNode* parent, const KeyType& key);
	bool eraseRR(MapNode* &v, MapNode* &parent);
	MapNode* TwoChildHelper(MapNode* &v);
	
	//Get helper functions
	bool getR(const KeyType& key, ValueType& value, MapNode *v) const;
	
	//Update helper
	bool updateR(const KeyType& key, const ValueType& value, MapNode *v);

	//3 argument get helper
	void ThreeGetHelper(int* &i, KeyType& key, ValueType& value, MapNode* v) const;

public:
	Map() : root(nullptr) { counter = 0; };  // Create an empty map (i.e., one with no key/value pairs)

	Map(const Map& rhs); //Copy Constructor

	~Map();  //Destructor for Maps

	const Map& operator=(const Map& rhs); // assignment operator

	bool empty() const; // Return true if the map is empty, otherwise false.

	int size() const { return counter; };   // Return the number of key/value pairs in the map.

	bool insert(const KeyType& key, const ValueType& value);
	// If key is not equal to any key currently in the map, and if the
	// key/value pair can be added to the map, then do so and return true.
	// Otherwise, make no change to the map and return false (indicating
	// that either the key is already in the map).

	bool update(const KeyType& key, const ValueType& value);
	// If key is equal to a key currently in the map, then make that key no
	// longer map to the value it currently maps to, but instead map to
	// the value of the second parameter; return true in this case.
	// Otherwise, make no change to the map and return false.

	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	// If key is equal to a key currently in the map, then make that key no
	// longer map to the value it currently maps to, but instead map to
	// the value of the second parameter; return true in this case.
	// If key is not equal to any key currently in the map then add it and 
	// return true. In fact this function always returns true.


	bool erase(const KeyType& key);
	// If key is equal to a key currently in the map, remove the key/value
	// pair with that key from the map and return true.  Otherwise, make
	// no change to the map and return false.

	bool contains(const KeyType& key) const;
	// Return true if key is equal to a key currently in the map, otherwise
	// false.

	bool get(const KeyType& key, ValueType& value) const;
	// If key is equal to a key currently in the map, set value to the
	// value in the map that that key maps to, and return true.  Otherwise,
	// make no change to the value parameter of this function and return
	// false.

	bool get(int i, KeyType& key, ValueType& value) const;
	// If 0 <= i < size(), copy into the key and value parameters the
	// key and value of one of the key/value pairs in the map and return
	// true.  Otherwise, leave the key and value parameters unchanged and
	// return false.  (See below for details about this function.)

	void swap(Map& other);
	// Exchange the contents of this map with the other one.
};

//Other Functions
bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif // !MAP_H