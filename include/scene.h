#ifndef LOBSTER_SCENE
#define LOBSTER_SCENE 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <iostream>
#include <vector> 
#include <string>
#include <bitset>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <mesh.h>
#include <graphics.h>
#include <entity.h>
#include <component.h>
#include <importantConstants.h>

/// <summary> ///
/// 	This struct acts as the "master catalog" of all things existing inside of a single
///			environment, or scene. Here you will find all the entities instatiated in
///			the scene, any scripts that may exist in the scene, and other
///			scene-global variables that may be accessed by any scripts/components
///			in the scene.
/// </summary> ///

struct Scene
{
	// PUBLIC ATTRIBUTES //
	std :: string name = "";
	unsigned short int activeEntities = 0;

	// ENTITY/COMPONENT CONTAINER //
	Entity entities[LOBSTER_MAX_ENTITIES] = { };
	char * components[LOBSTER_MAX_COMPONENTS][LOBSTER_MAX_ENTITIES] = { };	
};

// FUNCTION DEFINITIONS //
namespace sceneManagement
{
	/// <summary> ///
	///		This function returns a new scene with a given name.
	/// </summary> ///
	Scene createScene(std :: string inputName="MyScene");

	/// <summary> ///
	///		Returns an index to the entity nearest to Index 0 with a component mask
	///			equalling 0.
	/// </summary> ///
	entityID newEntityID(Scene * targetScene);

	/// <summary> ///
	///		Clears an entity's component mask to 0, effectively deleting it from the
	///			scene.
	/// </summary> ///
	void deleteEntity(Scene * targetScene, entityID entityIndex);

	/// <summary> ///
	///		This function adds a component of input type to input entity in the scene.
	/// </summary> ///
	void addComp
		(Scene * targetScene, entityID entityIndex, componentID compIndex, char * compPtr);

	/// <summary> ///
	///		Iterates through all entities in the scene, and draws them.
	/// </summary> ///
	void renderScene(Scene * targetScene, Pipeline * inputPipeline);
};

#endif
