#include "GameManager.h"

/** Constructor overloads */
/// Creates a game object at the origin (0, 0) with default values.
GameObject::GameObject() {
	mesh = nullptr;
	position = velocity = vec3(0.0f);
	rotationAxis = vec3(0.0f, 1.0f, 0.0f);
	rotation = rotationVelocity = 0.0f;
	scale = vec3(1.0f);
	mass = 1.0f;
	acceleration = vec3(0.0f);
	boundingBox = OBB();
	gravity = false;
	friction = false;
}

/// Creates a game object at a specified position with a starting velocity, rotation, and scale.
// @param triPtr a pointer to a GLfloat array describing a triangle
// @param position the spawn position of the GameObject
// @param velocity the starting velocity of the GameObject
// @param rotationAxis the axis of rotation of the GameObject
// @param rotation the starting rotation of the GameObject
// @param rotationVelocity the starting rotational velocity of the GameObject
// @param scale the starting scale of the GameObject
GameObject::GameObject(string _meshFile, vec3 _position, vec3 _velocity, vec3 _scale, vec3 _rotationAxis, float _rotation, float _rotationVelocity, float _mass) {
	// Ask the game manager to load our mesh
	GameManager::LoadMesh(_meshFile);

	// Assign variables
	mesh = GameManager::meshes[_meshFile];
	position = _position;
	velocity = _velocity;
	scale = _scale;
	rotationAxis = _rotationAxis;
	rotation = _rotation;
	rotationVelocity = _rotationVelocity;
	mass = _mass;
	acceleration = vec3(0.0f);
	gravity = false;
	friction = false;
	name = _meshFile;
	isDead = false;

	// Create bounding box
	boundingBox = OBB(mesh->GetVerts(), position);
	boundingBox.Rotate(rotationAxis, rotation);
}

/// Gameplay & physics functions
void GameObject::ApplyForce(vec3 force) {
	acceleration += force/mass;
}

/// Check if this collides with another game object by comparing their OBBs
bool GameObject::collidesWith(GameObject & other)
{
	return boundingBox.collidesWith(other.GetOBB());
}

/// Check if this collides with another OBB by comparing it to this object's OBB
bool GameObject::collidesWith(OBB & other)
{
	return boundingBox.collidesWith(other);
}

/** Engine Functions **/
/// Main physics update for GameObject
void GameObject::Update(float deltaTime) {
	// update physics vectors based on forces
	acceleration = vec3();
	if (friction) {
		velocity *= 0.98f; // general drag
	}
	if (gravity) {
		acceleration += GameManager::gravity; // gravity
	}

	// physics updates
	velocity += acceleration*deltaTime;
	position += velocity*deltaTime;
	rotation += rotationVelocity*deltaTime;

	// update the OBB
	boundingBox.center = position;
	boundingBox.Rotate(rotationAxis, rotationVelocity*deltaTime);
}

/// Main render call for GameObject
void GameObject::Render() {
	if(!isDead)
		this->mesh->Render(position, scale, rotationAxis, rotation);
}

/// Removes this game object from the world
void GameObject::Delete()
{
	GameManager::Delete(this);
}

/** Mutators and Accessors */
/// Returns private location
const vec3 GameObject::GetPosition()
{
	return position;
}

/// Returns the object's OBB
OBB GameObject::GetOBB()
{
	return boundingBox;
}

/// Updates the GameObject's mesh
// @newMesh		Pointer to a new data array that will be buffered next render
void GameObject::SetMesh(Mesh* newMesh) {
	mesh = newMesh;
}

/// Updates the object's position
// @newPos		The new position to move the game object to
void GameObject::SetPosition(vec3 newPos) {
	position = newPos;
}

/// Forces the object to a new velocity
// @newVel		The object's new velocity
void GameObject::SetVelocity(vec3 newVel) {
	velocity = newVel;
}

/// Changes the rotation axis of the game object
// @newRotAxis	The new rotation axis
void GameObject::SetRotationAxis(vec3 newRotAxis) {
	rotationAxis = newRotAxis;
}

/// Forces the object to a new rotation
// @newRot		The new rotation in radians around its rotation axis
void GameObject::SetRotation(float newRot) {
	rotation = newRot;
}

/// Updates the speed which the object rotates around its axis
// @newRotVel	The new velocity of rotation
void GameObject::SetRotationVelocity(float newRotVel) {
	rotationVelocity = newRotVel;
}

/// Changes the mass of the object, which makes forces apply less velocity
// @newMass		The object's new mass
void GameObject::SetMass(float newMass) {
	mass = newMass;
}

/// Sets the object's scale, which affects its rendering
// @newScale	The object's new scale, in the form of vec3(xScale, yScale, zScale)
void GameObject::SetScale(vec3 newScale) {
	scale = newScale;
}

/// Returns a pointer to the object's mesh
Mesh* GameObject::GetMesh() {
	return mesh;
}

/** Destructor */
GameObject::~GameObject(void) {
}
