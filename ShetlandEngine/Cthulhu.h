#pragma once
#include "GameObject.h"
#include "GameManager.h"

class Cthulhu : public GameObject
{
	public:
		// Constructor
		Cthulhu();

		// Public member data
		int AItype;
		
		// Public member functions
		void Update(float dt) override;

		// Destructor
		~Cthulhu();
};

