#include "GameObject.h"

/** Constructor overloads */
/**
	Creates a game object at the origin (0, 0) with default values.
*/
GameObject::GameObject() {
	this->mesh = nullptr;
	this->position = this->velocity = vec3(0.0f);
	this->rotationAxis = vec3(0.0f, 0.0f, 1.0f);
	this->rotation = this->rotationVelocity = 0.0f;
	this->scale = vec3(1.0f);
	this->acceleration = vec3(0.0f);
	this->mass = 1.0f;
	gravity = vec3(0.0f, -9.8f, 0.0f);

	active = false;
	bound = false;
	age = 0;
}
/**
	Creates a game object at a specified position.

	@param triPtr a pointer to a GLfloat array describing a triangle
	@param position the spawn position of the GameObject
*/
GameObject::GameObject(Mesh* meshPtr, vec3 position, float mass) {
	this->mesh = meshPtr;
	this->position = position;
	this->velocity = vec3(0.0f);
	this->rotationAxis = vec3(0.0f, 1.0f, 0.0f);
	this->rotation = 0.0f;
	this->rotationVelocity = 0.0f;
	this->scale = vec3(1.0f);
	this->acceleration = vec3(0.0f);
	this->mass = mass;
	gravity = vec3(0.0f, -9.8f, 0.0f);

	active = false;
	bound = false;
	age = 0;
}
/**
	Creates a game object at a specified position with a starting velocity.

	@param triPtr a pointer to a GLfloat array describing a triangle
	@param position the spawn position of the GameObject
	@param velocity the starting velocity of the GameObject
*/
GameObject::GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, float mass) {
	this->mesh = meshPtr;
	this->position = position;
	this->velocity = velocity;
	this->rotationAxis = vec3(0.0f, 1.0f, 0.0f);
	this->rotation = 0.0f;
	this->rotationVelocity = 0.0f;
	this->scale = vec3(1.0f);
	this->acceleration = vec3(0.0f);
	this->mass = mass;
	gravity = vec3(0.0f, -9.8f, 0.0f);

	active = false;
	bound = false;
	age = 0;
}
/**
	Creates a game object at a specified position with a starting velocity and rotation.

	@param triPtr a pointer to a GLfloat array describing a triangle
	@param position the spawn position of the GameObject
	@param velocity the starting velocity of the GameObject
	@param rotationAxis the axis of rotation of the GameObject
	@param rotation the starting rotation of the GameObject
	@param rotationVelocity the starting rotational velocity of the GameObject
*/
GameObject::GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, vec3 rotationAxis, float rotation, float rotationVelocity, float mass) {
	this->mesh = meshPtr;
	this->position = position;
	this->velocity = velocity;
	this->rotationAxis = rotationAxis;
	this->rotation = rotation;
	this->rotationVelocity = rotationVelocity;
	this->scale = vec3(1.0f);
	this->acceleration = vec3(0.0f);
	this->mass = mass;
	gravity = vec3(0.0f, -9.8f, 0.0f);

	active = false;
	bound = false;
	age = 0;
}
/**
	Creates a game object at a specified position with a starting velocity, rotation, and scale.

	@param triPtr a pointer to a GLfloat array describing a triangle
	@param position the spawn position of the GameObject
	@param velocity the starting velocity of the GameObject
	@param rotationAxis the axis of rotation of the GameObject
	@param rotation the starting rotation of the GameObject
	@param rotationVelocity the starting rotational velocity of the GameObject
	@param scale the starting scale of the GameObject
*/
GameObject::GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, vec3 rotationAxis, float rotation, float rotationVelocity, vec3 scale, float mass) {
	this->mesh = meshPtr;
	this->position = position;
	this->velocity = velocity;
	this->rotationAxis = rotationAxis;
	this->rotation = rotation;
	this->rotationVelocity = rotationVelocity;
	this->scale = scale;
	this->acceleration = vec3(0.0f);
	this->mass = mass;
	gravity = vec3(0.0f, -9.8f, 0.0f);

	active = false;
	bound = false;
	age = 0;
}

/** Gameplay & physics functions */
void GameObject::ApplyForce(vec3 force) {
	acceleration += force/mass;
}

/** Engine Functions **/
/** Main physics update for GameObject */
void GameObject::Update(float deltaTime) {
	if (active) {
		// don't update phyiscs if it's bound to the mouse
		if (!bound) {
			// update physics vectors based on forces
			acceleration *= 0.98; // general drag
			acceleration += gravity*deltaTime; // gravity

			// physics updates
			velocity += acceleration*deltaTime;
			position += velocity*deltaTime;
			rotation += rotationVelocity*deltaTime;

			// bounce off screen sides
			if (abs(position.x) >= 1) {
				// always drag on walls
				rotationVelocity *= .8f;
				acceleration *= .8f;
				velocity *= .8f;
				// only bounce when it passes wall
				if (abs(position.x) > 1) {
					velocity.x *= -1;
					position.x = round(position.x);
				}
			}
			// bound off screen top/bottom
			if (abs(position.y) >= 1) {
				// always drag on floor
				rotationVelocity *= .8f;
				acceleration *= .8f;
				velocity *= .8f;
				// only bounce when it passes floor
				if (abs(position.y) > 1) {
					velocity.y *= -1;
					position.y = round(position.y);
				}
			}

			// deactivate once it stops for a bit
			if (length(velocity) < 0.001f)  { age++; }
			if (age > 500)	{ active = false; }
		}

		// render
		Render();
	}
}
/** Main render call for GameObject */
void GameObject::Render() {
	this->mesh->Render(position, scale, rotationAxis, rotation);
}

/** Setters/Accessors */
// Returns location
vec3 GameObject::GetPosition()
{
	return position;
}
void GameObject::SetMesh(Mesh* newMesh) {
	mesh = newMesh;
}
void GameObject::SetPosition(vec3 newPos) {
	position = newPos;
}
void GameObject::SetVelocity(vec3 newVel) {
	// cap vector magnitude at 0.25f
	velocity = newVel;
}
void GameObject::SetRotationAxis(vec3 newRotAxis) {
	rotationAxis = newRotAxis;
}
void GameObject::SetRotation(float newRot) {
	rotation = newRot;
}
void GameObject::SetRotationVelocity(float newRotVel) {
	rotationVelocity = newRotVel;
}
void GameObject::SetMass(float newMass) {
	mass = newMass;
}
void GameObject::SetScale(vec3 newScale) {
	scale = newScale;
}

Mesh* GameObject::GetMesh() { return mesh; }

/** Destructor */
GameObject::~GameObject(void) {
}
