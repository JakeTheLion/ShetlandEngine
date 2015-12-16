#pragma once
#include "Mesh.h"
#include "OBB.h"

/** Game object class */
// Can store a mesh and has physics.
class GameWorld;
class GameObject
{
	public:
		// Constructors and Destructors
		GameObject(void);
		GameObject(Mesh* meshPtr, vec3 position, float mass);
		GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, float mass);
		GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, vec3 rotationAxis, float rotation, float rotationVelocity, float mass);
		GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, vec3 rotationAxis, float rotation, float rotationVelocity, vec3 scale, float mass);
		~GameObject(void);

		// Setters/Accessors
		void SetMesh(Mesh* newMesh);
		void SetPosition(vec3 newPos);
		void SetVelocity(vec3 newVel);
		void SetRotationAxis(vec3 newRotAxis);
		void SetRotation(float newRot);
		void SetMass(float newMass);
		void SetRotationVelocity(float newRotVel);
		void SetScale(vec3 newScale);
		const vec3 GetPosition();
		Mesh* GetMesh();

		// Gameplay & physics functions
		void ApplyForce(vec3 force);

		// Engine functions
		void Update(float deltaTime);
		void Render();

		// Public object control variables
		bool active;
		int age;

		// Static world variables
		vec3 gravity;

	protected:
		Mesh* mesh;
		vec3 acceleration, position, velocity, rotationAxis, scale;
		float rotation, rotationVelocity, mass;
};