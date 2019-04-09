#include "map.h"
//Other functions
void subtract(const Map& m1, const Map& m2, Map& result)
{
	Map BlackList;
	for (int i = 0; i < m1.size(); i++) //Iterate through each node to find duplicates to be added to blacklist
	{
		KeyType a;
		ValueType b;
		m1.get(i, a, b);
		if (m2.contains(a))
		{
			BlackList.insert(a, b);
		}
		if (!BlackList.contains(a)) //Add nodes that arent in the blacklist
			result.insert(a, b);
	}
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	Map BlackList, WhiteList;
	bool FinalResult = true;
	for (int i = 0; i < m1.size(); i++) //Iterate through each node in m1 to find duplicates that match or mismatch
	{									//If they match add to whitelist and insert to result otherwise add to blacklist and do not insert
		KeyType a;
		ValueType b;
		m1.get(i, a, b);
		if (m2.contains(a))
		{
			ValueType c;
			m2.get(a,c);
			if (b == c)
			{
				WhiteList.insert(a, b);
				result.insert(a, b);
			}
			else if (b != c)
			{
				BlackList.insert(a, 0);
				FinalResult = false;
			}
		}
		
	}

	for (int i = 0; i < m1.size(); i++) //Iterate through each item to be added that arent on both lists
	{
		KeyType a;
		ValueType b;
		m1.get(i, a, b);
		if (!BlackList.contains(a) && !WhiteList.contains(a))
			result.insert(a, b);
	}
	for (int i = 0; i < m2.size(); i++) //Iterate through each item to be added that arent on both lists
	{
		KeyType a;
		ValueType b;
		m2.get(i, a, b);
		if (!BlackList.contains(a) && !WhiteList.contains(a))
			result.insert(a, b);
	}
	if (FinalResult == false) //Check to see if a mismatched duplicate was found to return false
		return false;
	else
		return true;
}





//Copy Constructor
Map::Map(const Map& rhs)
{
	root = copyMap(rhs.root);
	counter = rhs.counter;
}

//Assignment Operator
const Map& Map::operator=(const Map& rhs)
{
	if (&rhs == this) { return *this; }
	//Clear current tree if there is anything
	destroyTree(root);
	//Begin to copy the tree into new tree
	//root = new WordNode;
	//root->count = rhs.root->count;
	//root->m_data = rhs.root->m_data;
	root = copyMap(rhs.root);
	counter = rhs.counter;
}

//Copy Constructor helper function
Map::MapNode* Map::copyMap(const MapNode* node)
{
	if (node == NULL) { return NULL; } //Empty tree or end the recursive traversal
	MapNode *temp = new MapNode;
	temp->m_key = node->m_key;			//Add new nodes
	temp->m_value = node->m_value;

	if (node->m_left != NULL)		//Traverse left or right to keep adding nodes
	{
		temp->m_left = copyMap(node->m_left);
	}
	else
		temp->m_left = NULL;
	if (node->m_right != NULL)
	{
		temp->m_right = copyMap(node->m_right);
	}
	else
		temp->m_right = NULL;
	return temp;
}

// Exchange the contents of this map with the other one.
void Map::swap(Map& other)
{
	MapNode *temp = other.root;
	int CounterTemp = other.counter;
	other.root = root;
	other.counter = counter;
	root = temp;
	counter = CounterTemp;
}
// Return true if the map is empty, otherwise false.
bool Map::empty() const { if (root == NULL) return true; else return false; }

// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map).
bool Map::insert(const KeyType& key, const ValueType& value)
{
	return Radd(root, key, value);
}
bool Map::Radd(MapNode* &node, const KeyType& key, const ValueType& value)
{
	if (node == NULL) //If root or and empty node dynamically create a node
	{
		node = new MapNode;
		node->m_key = key;
		node->m_value = value;
		node->m_left = NULL;
		node->m_right = NULL;
		counter++;
		return true;
	}
	if (key == node->m_key) //Do not add if node with the same key exists
		return false;
	if (key < node->m_key)
	{
		if (node->m_left != NULL) { Radd(node->m_left, key, value); } //If there is a node add a new one or traverse to the left if less
		else														//Less than current node
		{
			node->m_left = new MapNode;
			node->m_left->m_key = key;
			node->m_left->m_value = value;
			node->m_left->m_left = NULL;
			node->m_left->m_right = NULL;
			counter++;
			return true;
		}
	}
	if (key > node->m_key) {
		if (node->m_right != NULL) { Radd(node->m_right, key, value); }
		else
		{
			node->m_right = new MapNode;
			node->m_right->m_key = key;
			node->m_right->m_value = value;
			node->m_right->m_right = NULL;
			node->m_right->m_left = NULL;
			counter++;
			return true;
		}
	}

}

// Return true if key is equal to a key currently in the map, otherwise
// false.
bool Map::containsHelper(MapNode* node, const KeyType& key) const
{
	if (node == NULL) //Simple traversal to find key in map
		return false;
	if (node->m_key == key)
		return true;
	if (key < node->m_key) { return	containsHelper(node->m_left, key); }
	if (key > node->m_key) { return containsHelper(node->m_right, key); }

}
bool Map::contains(const KeyType& key) const
{
	return containsHelper(root, key);
}

//destructor function
Map::~Map() { destroyTree(root); }
//Function to destroy the tree recursively
void Map::destroyTree(MapNode *node)
{
	if (node == NULL)
		return;
	destroyTree(node->m_left);
	destroyTree(node->m_right);
	delete node;
}
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
bool Map::erase(const KeyType& key)
{
	if (eraseR(root, nullptr, key))
	{
		counter--;
		return true;
	}
	else
		return false;
}
bool Map::eraseR(MapNode* v, MapNode* parent, const KeyType& key)
{
	if (v->m_key == key)
	return	eraseRR(v, parent);
	else if (key < v->m_key)
		eraseR(v->m_left, v, key);
	else if (key > v->m_key)
		eraseR(v->m_right, v, key);
	return false;
}
bool Map::eraseRR(MapNode* &v, MapNode* &parent)
{

	//No children node deletion
	if (v->m_left == NULL && v->m_right == NULL)
	{
		//If node was the root node
		if (parent == NULL)
		{
			root = NULL;
			delete v;
			return true;
		}
		//If the node was on the left node
		if (v->m_key < parent->m_key)
		{
			parent->m_left = NULL;
			delete v;
			return true;
		}
		//If the node was on the right node
		if (v->m_key > parent->m_key)
		{
			parent->m_right = NULL;
			delete v;
			return true;
		}
	}
	//One child node deletion
	if (v->m_left == NULL && v->m_right != NULL)
	{
		//If root
		if (parent == NULL)
		{
			root = v->m_right;
			delete v;
			return true;
		}
		//If the node was on the left node
		if (v->m_key < parent->m_key)
		{
			parent->m_left = v->m_right;
			delete v;
			return true;
		}
		//If the node was on the right node
		if (v->m_key > parent->m_key)
		{
			parent->m_right = v->m_right;
			delete v;
			return true;
		}
	}
	if (v->m_right == NULL && v->m_left != NULL)
	{
		//If root
		if (parent == NULL)
		{
			root = v->m_left;
			delete v;
			return true;
		}
		//If the node was on the left node
		if (v->m_key < parent->m_key)
		{
			parent->m_left = v->m_left;
			delete v;
			return true;
		}
		//If the node was on the right node
		if (v->m_key > parent->m_key)
		{
			parent->m_right = v->m_left;
			delete v;
			return true;
		}
	}
	//Two children node deletion
	if (v->m_right != NULL && v->m_left != NULL)
	{
		MapNode* temp = new MapNode; //dynamically allocate a new node to be accessible outside of function
		MapNode* other;
		other = TwoChildHelper(v);
		temp->m_value = other->m_value;
		temp->m_key = other->m_key;
		erase(temp->m_key);
		v->m_value = temp->m_value;
		v->m_key = temp->m_key;
		delete temp;
	}


}
//Funtion to find the largest left child for two child node deletion
Map::MapNode* Map::TwoChildHelper(MapNode* &v)
{	
	if (v->m_right == NULL)
		return v;
	TwoChildHelper(v->m_right);
}



// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
bool Map::get(const KeyType& key, ValueType& value) const { return	getR(key, value, root); }
bool Map::getR(const KeyType& key, ValueType& value, MapNode *v) const
{
	//Not sure if this is the right solution but here we go!
	if (v == NULL)
		return false;
	if (v->m_key == key)
	{
		value = v->m_value;
		return true;
	}
	if (key < v->m_key) { getR(key, value, v->m_left); }
	if (key > v->m_key) { getR(key, value, v->m_right); }
	

}
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
bool Map::get(int i, KeyType& key, ValueType& value) const 
{
	if (i < 0 || i > this->size())
		return false;
	if (root == NULL)
		return false;
	int *temp;
	temp = new int; //Accessible counter to be dynamically allocated
	*temp = i;

	ThreeGetHelper(temp, key, value, root);
	delete temp;

	return true;
}
void Map::ThreeGetHelper(int* &i, KeyType& key, ValueType& value, MapNode* v) const
{
	if (v == NULL)
		return;
	if (*i == this->size()) //Decrement counter for each iteration until equal to size
		return;
	key = v->m_key;
	value = v->m_value;
	*i += 1;

	ThreeGetHelper(i, key, value, v->m_left);
	ThreeGetHelper(i, key, value, v->m_right);
	
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
bool Map::update(const KeyType& key, const ValueType& value) 
{
	return updateR(key, value, root);
}
bool Map::updateR(const KeyType& key, const ValueType& value, MapNode *v)
{
	if (v == NULL)
		return false;
	if (v->m_key == key)
	{
		v->m_value = value;
		return true;
	}
	if (key < v->m_key) { updateR(key, value, v->m_left); }
	if (key > v->m_key) { updateR(key, value, v->m_right); }
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map then add it and 
// return true. In fact this function always returns true.
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (update(key, value))
		return true;
	else
		return insert(key, value);
}

