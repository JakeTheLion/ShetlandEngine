#include "Node.h"


/// Default ctor
Node::Node()
{
	count = 0;					// number of colliders in the node
	capacity = 4;				// max number of colliders node can hold
	isLeaf = true;				// whether this node is a leaf on the tree
	for (int i = 0; i < 2; ++i)	// set all child nodes to nullptrs
		for (int j = 0; j < 2; ++j)
			for (int k = 0; k < 2; ++k)
				children[i][j][k] = nullptr;
}

/// Parameterized ctor
//@C	Center of the node's bin box
//@E	Half-width of he node's bin box
Node::Node(vec3 C, vec3 E)
{
	binBox = OBB(C, E);			// the node's bin box collider
	count = 0;					// number of colliders in the node
	capacity = 4;				// max number of colliders node can hold
	isLeaf = true;				// whether this node is a leaf on the tree
	for (int i = 0; i < 2; ++i)	// set all child nodes to nullptrs
		for (int j = 0; j < 2; ++j)
			for (int k = 0; k < 2; ++k)
				children[i][j][k] = nullptr;
}

/// collidesWith: checks if the passed collider hits one of these colliders or a child's
/// Passes object down to children if this node isn't a leaf of the tree
// @o	The other collider to check against
bool Node::collidesWith(OBB* o) {
	// First, make sure the passed collider even overlaps this node's bin
	if (!o->collidesWith(binBox)) {
		return false;
	}

	// Handle collision checks differently depending on if the node is a leaf
	if (isLeaf) {
		// It's a leaf node - check for collisions with each contained collider
		for (size_t i = 0; i < contents.size(); ++i) {
			if (o->collidesWith(*contents[i])) {
				return true;
			}
		}

		// If we reach this point, none collided, so there's no collision
		return false;
	}
	// If this isn't a leaf, we pass the collision check down to the children
	else {
		// Loop and check collisions with each child
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				for (int k = 0; k < 2; ++k)
					if (children[i][j][k]->collidesWith(o))
						return true;
					
		return false;
	}
}

/// add: attempts to add the passed collider to the node's bin box
/// Passes object down to children if this node isn't a leaf of the tree
// @o	The collider to add to the bin box
void Node::add(OBB* o) {
	// First, make sure the passed collider even overlaps this node's bin
	if (!o->collidesWith(binBox)) {
		return;
	}

	// Branch if we need to
	if (count >= capacity)
		branch();

	// Handle adding colliders differently depending on if the node is a leaf
	if (isLeaf) {
		// It's a leaf node - just add it to this collider and increment the count
		contents.push_back(o);
		++count;
	}
	// If this isn't a leaf, we pass the collider down to the children for adding
	else {
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				for (int k = 0; k < 2; ++k)
					children[i][j][k]->add(o);
	}
}

/// branch: splits the node if it is over capacity
void Node::branch() {
	// This node is not a leaf anymore!
	isLeaf = false;

	// Make the new child nodes
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			for (int k = 0; k < 2; ++k) {
				// Multiplier vector, gets the offset from the center to create new child at
				vec3 m = vec3(-0.5f + i, -0.5f + j, -0.5f + k);
				// The center of the new child, at this one's center offset by its half-widths/2 in +/- XYZ
				vec3 newC = vec3(binBox.center.x + binBox.e.x*m.x, binBox.center.y + binBox.e.x*m.y, binBox.center.z + binBox.e.z*m.z);
				// Create the new child
				children[i][j][k] = new Node(newC, binBox.e * 0.5f);

				// Add all of this node's colliders to the new child
				for (size_t ii = 0; ii < contents.size(); ++ii) {
					children[i][j][k]->add(contents[ii]);
				}
			}
		}
	}

	// Clear out the references
	contents.clear();

	// This node itself stores nothing now
	count = 0;
}

Node::~Node()
{
	// Loop and delete children
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			for (int k = 0; k < 2; ++k) {
				if (children[i][j][k] != nullptr) {
					delete children[i][j][k];
				}
			}
		}
	}
}
