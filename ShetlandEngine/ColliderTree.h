#pragma once
#include "Node.h"

class ColliderTree
{
	public:
		// Constructors
		ColliderTree();
		ColliderTree(vec3 C, vec3 E);

		// Public member functions
		bool collidesWith(OBB*);		// checks if root node collides with collider
		void add(OBB*);					// add collider to the root node

		// Destructor
		~ColliderTree();

	private:
		// Private member data
		vec3 c;							// center
		vec3 e;							// half-widths
		Node* root;						// the root node of the tree
};

