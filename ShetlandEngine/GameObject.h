#pragma once
#include "Mesh.h"
#include "OBB.h"

/** Game object class */
// Can store a mesh and has physics.
class GameObject
{
	public:
		// Constructors and Destructors
		GameObject(void);
		GameObject(string _meshFile,									   // pointer to the object's mesh
				   vec3 position = vec3(),								   // game object's starting position
				   vec3 velocity = vec3(),								   // game object's starting velocity
				   vec3 scale = vec3(1.0f),								   // scale along each axis (x/y/z scale = x/y/z/ of vec3)
				   vec3 rotationAxis = vec3(0.0f, 1.0f, 0.0f),			   // axis the game object will rotate around
				   float rotation = 0.0f,								   // object's starting rotation in radians around its rotation axis
				   float rotationVelocity = 0.0f,						   // rotation in radians per tick around its rotation axis
				   float mass = 1.0f);									   // mass, affects strength of force impulses
		~GameObject(void);

		// Mutators
		void SetMesh(Mesh* newMesh);
		void SetPosition(vec3 newPos);
		void SetVelocity(vec3 newVel);
		void SetRotationAxis(vec3 newRotAxis);
		void SetRotation(float newRot);
		void SetMass(float newMass);
		void SetRotationVelocity(float newRotVel);
		void SetScale(vec3 newScale);

		// Accessors
		const vec3 GetPosition();
		OBB GetOBB();
		Mesh* GetMesh();

		// Gameplay & physics functions
		void ApplyForce(vec3 force);
		bool collidesWith(GameObject& other);
		bool collidesWith(OBB& other);

		// Engine functions
		void virtual Update(float deltaTime);
		void virtual Render();
		void Delete();

		// Public object control variables
		bool gravity;
		bool friction;

	protected:
		OBB boundingBox;
		Mesh* mesh;
		vec3 acceleration, position, velocity, rotationAxis, scale;
		float rotation, rotationVelocity, mass;
};