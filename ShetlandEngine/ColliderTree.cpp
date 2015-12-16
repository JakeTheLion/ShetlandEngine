#include "ColliderTree.h"



ColliderTree::ColliderTree() {
	// Create basic root
	root = nullptr;

	// Default center/half width
	c = vec3(0.0f);
	e = vec3(1.0f);
}

ColliderTree::ColliderTree(vec3 C, vec3 E) {
	// Create root
	root = new Node(C, E);

	// Store center/half width
	c = C;
	e = E;
}

/// collidesWith: sends the passed collider to the root node for a collision check
// @o	The other collider to check against
GameObject* ColliderTree::collidesWith(GameObject* o) {
	return (root->collidesWith(o));
}

/// add: sends the passed collider to the rood node to attempt to add it
// @o	The collider to add
void ColliderTree::add(GameObject* o) {
	root->add(o);
}

ColliderTree::~ColliderTree() {
	// Delete root if it's not a nullptr
	//if (root != nullptr) {
	//	delete root;
	//}
}
