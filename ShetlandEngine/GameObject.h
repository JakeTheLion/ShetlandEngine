#include "Mesh.h"
#pragma once

/** Game object class */
// Can store a mesh (currently just a single shape) and
// has physics. Can be bound to mouse and dies after a
// period of inactivity
class GameObject
{
	public:
		/** Constructors and Destructors */
		GameObject(void);
		GameObject(Mesh* meshPtr, vec3 position, float mass);
		GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, float mass);
		GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, vec3 rotationAxis, float rotation, float rotationVelocity, float mass);
		GameObject(Mesh* meshPtr, vec3 position, vec3 velocity, vec3 rotationAxis, float rotation, float rotationVelocity, vec3 scale, float mass);
		~GameObject(void);
		/** Setters/Accessors */
		void SetMesh(Mesh* newMesh);
		void SetPosition(vec3 newPos);
		void SetVelocity(vec3 newVel);
		void SetRotationAxis(vec3 newRotAxis);
		void SetRotation(float newRot);
		void SetMass(float newMass);
		void SetRotationVelocity(float newRotVel);
		void SetScale(vec3 newScale);
		Mesh* GetMesh();

		/** Gameplay & physics functions */
		void ApplyForce(vec3 force);
		/** Engine functions */
		void Update(float deltaTime);
		void Render();
		/** Public object control variables */
		bool active, bound;
		int age;
		/** Static world variables */
		vec3 gravity;

	private:
		Mesh* mesh;
		vec3 acceleration, position, velocity, rotationAxis, scale;
		float rotation, rotationVelocity, mass;
};