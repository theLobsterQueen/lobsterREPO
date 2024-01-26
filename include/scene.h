#ifndef LOBSTER_SCENE
#define LOBSTER_SCENE 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <iostream>
#include <vector> 
#include <string>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <mesh.h>
#include <entity.h>
#include <component.h>
#include <importantConstants.h>
#include <window.h>
#include <transform.h>

// STRUCT PROTOTYPES //
struct EngineCore;

/// <summary> ///
///		This struct serves as the "player" viewpoint for the scene. It holds the
///			positional offset that the player is at which will affect the rendering
///			of the objects in the scene, and may also carry any other data relevant
///			to the player's perspective.
/// </summary> ///
struct Viewport
{
	GLfloat playerPos[3] = {0};
};

/// <summary> ///
/// 	This struct acts as the "master catalog" of all things existing inside of a single
///			environment, or scene. Here you will find all the entities instatiated in
///			the scene, any scripts that may exist in the scene, and other
///			scene-global variables that may be accessed by any scripts/components
///			in the scene.
/// </summary> ///

struct Scene
{
	// BASIC ATTRIBUTES //
	std :: string name = "";
	unsigned short int activeEntities = 0;
	Viewport * viewRef = new Viewport;

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
	Scene * createScene(std :: string inputName="MyScene");

	/// <summary> ///
	///		This function sets the engine's current scene to input target scene.
	/// </summary> ///
	void useScene(EngineCore * core, Scene * targetScene);

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
	// PARAMETERS //
	(
		Scene * targetScene, entityID entityIndex, 
		componentID compIndex, compPtr inputCompPtr
	);

	/// <summary> ///
	///		Iterates through all entities in the scene, and draws them.
	/// </summary> ///
	void renderScene(Scene * targetScene, Pipeline * inputPipeline);
};

#endif
