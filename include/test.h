#ifndef LOB_TEST_H
#define LOB_TEST_H 1

// INCLUDES AND INITIALIZATIONS //

// LOBSTER INCLUDES //
#include <engineCore.h>
#include <entity.h>
#include <transform.h>
#include <scene.h>

namespace testFuncs
{
	/// <summary> ///
	///		A test function used to neatly process input and move an entity
	///			about the scene.
	/// </summary> ///

	void processInput(EngineCore * core, entityID inputID);
};

#endif
