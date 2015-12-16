#pragma once
#include "OBB.h"
#include <vector>
using namespace std;

class Node
{
	public:
		// Constructors
		Node();
		Node(vec3 C, vec3 E);

		// Public member functions
		bool collidesWith(OBB*);
		void add(OBB*);
		void branch();

		// Destructor
		~Node();

	private:
		// Private member data
		vector<OBB*> contents;			// node's stored colliders
		OBB binBox;						// the collider of the node's bin
		int count;						// number of colliders in the node
		int capacity;					// max number of colliders node can hold
		bool isLeaf;					// whether this node is a leaf on the tree
		Node* children[2][2][2];		// the child branches of this node
};

