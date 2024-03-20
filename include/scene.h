#ifndef LOBSTER_SCENE
#define LOBSTER_SCENE 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <iostream>
#include <vector> 
#include <string>
#include <fstream>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <mesh.h>
#include <entity.h>
#include <importantConstants.h>
#include <window.h>
#include <transform.h>
#include <camera.h>
#include <light.h>
#include <utilities.h>
#include <script.h>
#include <script.h>
#include <APIUtils.h>
#include <app.h>

// STRUCT PROTOTYPES //
struct EngineCore;

/// <summary> ///
/// 	This struct acts as the "master catalog" of all things existing inside of a single
//			environment, or scene. Here you will find all the entities instatiated in
///			the scene, any scripts that may exist in the scene, and other
///			scene-global variables that may be accessed by any scripts/components
///			in the scene.
/// </summary> ///

struct Scene
{
	// BASIC ATTRIBUTES //
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

	Scene * createScene(std :: string inputName="MyScene");

	/// <summary> ///
	///		This function sets the engine's current scene to input target scene.
	/// </summary> ///

	void changeScene(Scene * targetScene);

	/// <summary> ///
	/// 	Grabs and returns the first camera entity found in the scene.
	/// </summary> ///
	bool getCameraEntityID(Scene * targetScene, entityID * entHolder);
	
	/// <summary> ///
	///		Returns an index to the entity nearest to Index 0 with a component mask
	///			equalling 0.
	/// </summary> ///

	entityID newEntityID(Scene * targetScene, std :: string entityName);

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

	void renderScene(Scene * targetScene);

	/// <summary> ///
	///		Returns a array of all entities with input component.
	/// </summary> ///

	std :: vector<entityID> sceneView(Scene * inputScene, componentID compID);

	/// <summary> ///
	///		Saves the scene as a .lob file to the input directory.
	/// </summary> ///

	void saveScene(Scene * inputScene);

	/// <summary> ///
	///		Loads a scene from input relative path.
	/// </summary> ///

	Scene * loadScene(std :: string scenePath);

	/// <summary> ///
	///		The prints the name of every active entity in input scene, and
	///			list all of its components.
	/// </summary> ///

	void sceneOut(Scene * inputScene);

	/// <summary> ///
	///		Calls the update function on all scripts existing in the scene.
	/// </summary> ///

	void updateScene(Scene * inputScene, float deltaTime);

	/// <summary> ///
	///		This sets the value of a given item by a given entry in returned data dictionarys.
	/// </summary> ///

	template <class T>
	void setData(pybind11 :: dict retValue, std :: string entryName, T * data)
	{
		try
			{ (*data) = retValue[entryName.c_str()].cast<T>(); }
		catch(pybind11 :: error_already_set error)
			{;}
	}
};

#endif
