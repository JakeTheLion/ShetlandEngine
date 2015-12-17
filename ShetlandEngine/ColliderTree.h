#pragma once
#include "Node.h"

class ColliderTree
{
	public:
		// Constructors
		ColliderTree();
		ColliderTree(vec3 C, vec3 E);

		// Public member functions
		GameObject* collidesWith(GameObject*);	// checks if a GameObject collision occurs within the tree
		void add(GameObject*);					// add collider to the root node
		void empty();							// empties the octree

		// Destructor
		~ColliderTree();

	private:
		// Private member data
		vec3 c;							// center
		vec3 e;							// half-widths
		Node* root;						// the root node of the tree
};

