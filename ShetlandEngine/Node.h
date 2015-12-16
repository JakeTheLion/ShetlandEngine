#pragma once
#include "OBB.h"
#include <vector>
using namespace std;

class Node
{
	public:
		// Constructors
		Node();
		Node(const Node &) = delete;
		Node(vec3 C, vec3 E);

		// Accessors
		vector<OBB*> GetContents() const { return contents; }
		OBB GetBinBox() const { return binBox; }
		int GetCount() const { return count; }
		int GetCapacity() const { return capacity; }
		bool GetLeaf() const { return isLeaf; }

		// Public member functions
		Node& operator = (const Node &) = delete;
		bool collidesWith(OBB*);
		void add(OBB*);
		void branch();

		// Public member data
		Node* children[2][2][2];		// the child branches of this node

		// Destructor
		~Node();

	private:
		// Private member data
		vector<OBB*> contents;			// node's stored colliders
		OBB binBox;						// the collider of the node's bin
		int count;						// number of colliders in the node
		int capacity;					// max number of colliders node can hold
		bool isLeaf;					// whether this node is a leaf on the octree - has no children
};

