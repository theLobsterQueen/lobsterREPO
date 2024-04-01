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
#include <entity.h>

// VENDOR INCLUDES //
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp> 	// glm :: perspective
#include <glm/ext/matrix_transform.hpp> 	// glm :: translate/rotate/scale/lookAt
#include <glm/gtc/type_ptr.hpp> 			// glm :: value_ptr
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/string_cast.hpp>			// glm :: to_string
#include <pybind11/stl.h>

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
	///		Copies an entity fully from one ID slot to another.
	/// </summary> ///

	void copyEntity(Scene * inputScene, entityID copyID);

	/// <summary> ///
	///		This function adds a component of input type to input entity in the scene.
	/// </summary> ///

	void addComp
	(
		Scene * targetScene, entityID entityIndex, 
		componentID compIndex, compPtr inputCompPtr
	);

	void removeComp
	(
	 	Scene * targetScene, entityID entityIndex,
		componentID compIndex
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

	void saveScene(Scene * inputScene, std :: string alternateName = "");

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
	///		Pulls all orders pushed from the scripting API. 
	/// </summary> ///

	void pullOrders(Scene * inputScene);
};

#endif
#ifndef LOBSTER_API_UTILS
#define LOBSTER_API_UTILS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <string>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <scene.h>
#include <input.h>
#include <mesh.h>

// DECLARES NAMESPACE-GLOBAL VARIABLES //
namespace APIGlobals
{
	extern pybind11 :: module coremodule;
	extern pybind11 :: module inputmodule;
	extern std :: string workingPath;
}

#endif
#ifndef LOBSTER_WINDOW_H
#define LOBSTER_WINDOW_H 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <string>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <globals.h>

/// <summary> ///
///		This file defines functions that change the appearence of the engine in some way,
///			but do not fit within other objects such as the render pipeline
///			or individual meshes.
/// </summary> ///

namespace windowManagement
{
	// FUNCTION DEFINITIONS //

	/// <summary> ///
	///		Changes the title of the main window.
	/// </summary> ///
	void changeTitle(SDL_Window * window, std :: string windowName);
};

#endif
#ifndef LOBSTER_APP
#define LOBSTER_APP 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <globals.h>
#include <window.h>
#include <mesh.h>
#include <scene.h>
#include <transform.h>
#include <utilities.h>
#include <importantConstants.h>
#include <math.h>
#include <light.h>
#include <vendorIncludes.h>
#include <ui.h>
#include <APIUtils.h>
#include <script.h>

// STRUCT PROTOTYPES //
struct Scene;

namespace appManagement
{
	// FUNCTION DEFINITIONS //

	/// <summary> ///
	///		Does all the initialization/cleanup work that main.cpp used to do.
	/// </summary> ///

	void initialize();
	void cleanup();

	/// <summary> ///
	///		This setups a base empty scene.
	/// </summary> ///
	void begin(std :: string sceneLoad = "");

	/// <summary> ///
	///		Sets up the working directory, initializes the pybind11 interpreter, and
	///			prepares the Python context.
	/// </summary> ///

	void initializeAPI();

	/// <summary> ///
	///		Iterates through all scripts in the "script" directory, and executes their code.
	/// </summary> ///

	void compileScripts();

	/// <summary> ///
	///		Calls the "_start" function on all scripts currently in
	///			the scene.
	/// </summary> ///

	void startScripts();

	/// <summary> ///
	///		When called, puts the engine into the "control loop." This loop will
	///			continually parse input, render meshes, and so on, until the program
	///			shuts down. 
	/// </summary> ///

	void run();

	/// <summary> ///
	///		This populates the editor with a base screen.
	/// </summary> ///

	Scene * createBaseScene();

	/// <summary> ///
	///		Updates the general game state by processing SDL2 events.
	/// </summary> ///

	void update();
};

#endif
#ifndef LOBSTER_UTILS
#define LOBSTER_UTILS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <input.h>
#include <light.h>
#include <importantConstants.h>

// STRUCT PROTOTYPES //
struct InputState;
struct Entity;

/// <summary> ///
///		These classes are used to debug GLSL shaders and OpenGL shader program objects.
///			Source: https://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php
/// </summary> ///

void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);
void checkErrors();

/// <summary> ///
///		Functions for reducing the amount of wet code in the input processor
///			on the update function.
/// </summary> ///

void mouseButtonProcess(InputState * inputState, SDL_Event inputEvent, bool setTrue);
void modifierKeyProcess(InputState * inputState, SDL_Event inputEvent, bool setTrue);

std :: string compToString(componentID compID);
componentID stringToComp(std :: string compName);
compPtr constructComp(componentID compID);
compPtr copyComp(componentID compID, compPtr copySrc);
bool hasComp(Entity inputEnt, componentID compID);

#endif
#ifndef LOBSTER_UI_H 
#define LOBSTER_UI_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <filesystem>
#include <cstdio>

// LOBSTER INCLUDES //
#include <globals.h>
#include <vendorIncludes.h>
#include <importantConstants.h>
#include <entity.h>
#include <transform.h>
#include <scene.h>
#include <app.h>

namespace uiManagement
{
	/// <summary> ///
	///		This file draws the basic overlay for the editor, containing
	///			data from the current scene.
	/// </summary> ///

	void drawEditorUI();

	/// <summary> ///
	///		Centers the string in targetBase around a text line with
	///			input margin.
	/// </summary> ///

	std :: string centeredString(const char * targetBase, unsigned margin);


	/// <summary> ///
	///		Continually waits for the player to input text, and then returns the
	///			text to the caller once it is submitted.
	/// </summary> ///

	bool getInputString(std :: string labelString, std :: string& outputData);

	/// <summary> ///
	///		Quick utility function to add delete buttons for components.
	/// </summary> ///

	void deleteButton(componentID compID);
};


#endif
#ifndef LOBSTER_ENTITY
#define LOBSTER_ENTITY 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <vector>
#include <bitset>
#include <string>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <globals.h>

// STRUCT PROTOTYPES //
struct Scene;

/// <summary> ///
///		This struct is the core to how the engine operates: an entity holds a key
///			and a bitmask which states whether or not it is holding a 
///			certain component of some type in each bit of the mask.
/// </summary> ///
struct Entity
{
	public :
		entityID ID;
		componentMask mask = 0;
		std :: string name;
};

// DELAYED INCLUDES TO RESOLVE COMPILER ERRORS //
#include <scene.h>

// FUNCTION DECLARATIONS //
namespace entityHandler
{
	void processOrder(std :: string orderName, entityID entID, std :: vector<pybind11 :: object>& params);
}

#endif
#ifndef LOBSTER_CAMERA_H
#define LOBSTER_CAMERA_H 1

// INCLUDES AND INIITALIZATIONS //

// LOBSTER INCLUDES //
#include <graphics.h>
#include <math.h>
#include <cmath>
#include <transform.h>

// VENDOR INCLUDES //
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp> 	// glm :: perspective
#include <glm/ext/matrix_transform.hpp> 	// glm :: translate/rotate/scale/lookAt

// PROTOTYPE STRUCTS //
struct Transform;

/// <summary> ///
///		This component is used to render a view of a scene from a specific point
///			to a specific target.
///		(TODO) Make the camera able to render to targets other than the main window.
/// </summary> ///

struct Camera
{
	/// <summary> ///
	///		The aspect ratio of the target viewport.
	/// </summary> ///

	float aspectRatio;

	/// <summary> ///
	///		This horizontal and vertical field of view's for the camera.
	///			A larger FOV will reduce the size of all objects in the screen,
	///			but will allow the viewer to see more of the scene at any given
	///			time.
	///
	///		The horizontal field of view can be set manually, but the 
	///			vertically field of view is automatically determined by the
	///			value of the horizontal field of view and the aspect ratio
	///			of the target viewport.
	/// </summary> ///

	float FOV;

	/// <summary> ///
	///		The two clipping distance values for the camera.
	/// </summary> ///

	float near = 0.1f;
	float far = 100.0f;

	/// <summary> ///
	///		Unit vectors used for calculating the view matrix of the camera.
	/// </summary> ///

	std :: vector<float> look = { 0.0f, 0.0f, -1.0f };
	std :: vector<float> up = { 0.0f, 1.0f, 0.0f };
	std :: vector<float> right = { 1.0f, 0.0f, 0.0f };

	/// <summary> ///
	///		The base pipeline this camera renders with (this can be changed
	///			if a mesh uses a special pipeline.
	/// </summary> ///

	Pipeline * curPipelineRef;
};

namespace cameraHandler
{
	/// <summary> ///
	///		Creates a new camera component, with a FOV of inputFOV value.
	///			When rendered to, it will render objects using input pipeline.
	/// </summary> ///

	Camera * createCamera
		(float inputAspect = 0.0f, Pipeline * inputPipeline = nullptr);

	/// <summary> ///
	///		Returns the view matrix for the camera when called. Can be used in
	///			combination with the projection matrix to render dynamic
	///			3D scenes.
	/// </summary> ///
	
	glm :: mat4x4 getWorldViewMatrix(Camera * camera, Transform * camTrans);

	/// <summary> ///
	///		Zooms the camera in/out by decreasing/increasing the camera's FOV.
	/// </summary> ///

	void zoom(Camera * camera, float zoomDelta);
};

#endif
#ifndef LOBSTER_MATH_H
#define LOBSTER_MATH_H 1

/// <summary> ///
///		This file contains a collections of structs and systems to facilitate
///			mathematical operations in the engine backend. 
///
///		NOTE: I manually wrote all this stuff so that I could understand it better.
///			I despise black-boxes, especially in something so critical to an engine's
///			operation like 3D math.
/// </summary> ///

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <iostream> 
#include <vector>
#include <stdlib.h>
#include <cmath>

// CLASS PROTOTYPES //
class LobMatrix;

// FUNCTION DEFINITIONS //
namespace math
{
	float dot(LobMatrix matY, LobMatrix matX, unsigned int yIndex, unsigned int xIndex);
	float dot(std :: vector<float> x, std :: vector<float> y);
	std :: vector<float> scaleVec(std :: vector<float> x, float s);
	std :: vector<float> cross(std :: vector<float> x, std :: vector<float> y);
	std :: vector<float> normalize(std :: vector<float> x);
	std :: vector<float> vecByMat(std :: vector<float> lhs, LobMatrix rhs);
	LobMatrix identityMatrix();
	LobMatrix inverse(LobMatrix inputMat);
	LobMatrix rotateMatrix(std :: vector<float> axis, float inputAngle);
	float toRadians(float x);
};

// STRUCT DEFINITIONS //

/// <summary> ///
///		Matrix, organized in column-major order to better translate to GLSL.
/// </summary> ///

class LobMatrix
{
	public :
		// CONSTRUCTOR/DESTRUCTOR ARGUMENTS //
		LobMatrix

		// PARAMETERS //
		(
			std :: vector<float> inputData, 
			unsigned int inputNumCols, unsigned int inputNumRows
		)

		{
			// NO-THOUGHT VARIABLE ASSIGNMENTS //
			numCols = inputNumCols;
			numRows = inputNumRows;

			// ASSERTS VALIDITY OF INPUT //
			if(inputData.size() != (numCols * numRows))
			{
				std :: cout << "ERROR! ATTEMPTED TO CREATE MATRIX DATA WITH VECTOR THAT "
					"WAS NOT THE SAME SIZE AS THE DIMENSIONS OF THE MATRIX!" 
					<< std :: endl;
				return;
			}

			// IF VALID, ASSIGNS DATA //
			data = inputData;
		}

		// CONSTRUCTOR WITH NO PRESET DATA //
		LobMatrix(unsigned int inputNumCols, unsigned int inputNumRows)
		{
			// NO-THOUGHT VARIABLE ASSIGNMENTS //
			numCols = inputNumCols;
			numRows = inputNumRows;

			unsigned int size = numCols * numRows;
			for(int i = 0; i < size; i++)
				data.push_back(0);
		}

		// PUBLIC METHODS //
		void out()
		{
			// PRINTS ENTRY GUARD //
			std :: cout << "/";
			for(int i = 0; i < numRows * 2; i++)
				std :: cout << "-"; 
			std :: cout << "\\" << std :: endl;

			for(int y = 0; y < numRows; y++)
			{
				std :: cout << " ";
				for(int x = 0; x < numCols; x++)
					std :: cout << getPoint(y, x) << " ";
				std :: cout << std :: endl;
			}

			std :: cout << "\\";
			for(int i = 0; i < numRows * 2; i++)
				std :: cout << "-"; 
			std :: cout << "/" << std :: endl;
		}

		// GET/SET METHODS //
		std :: vector<float> getVec()
			{ return data; }

		float * getData()
			{ return data.data(); }

		unsigned int getCols()
			{ return numCols; }

		unsigned int getRows()
			{ return numRows; }

		std :: vector<float> getRow(unsigned int rowIndex)
		{
			std :: vector<float> rowVec;
			for(int i = 0; i < numCols; i++)
				rowVec.push_back(data[(numCols * rowIndex) + i]);

			return rowVec;
		}

		std :: vector<float> getCol(unsigned int colIndex)
		{
			std :: vector<float> colVec;
			for(int i = 0; i < numRows; i++)
				colVec.push_back(data[(numRows * colIndex) + i]);

			return colVec;
		}

		float getPoint(unsigned int y, unsigned int x)
			{ return data[(numRows * x) + y]; }

		void setPoint(unsigned int y, unsigned int x, float value)
			{ data[(numRows * x) + y] = value; }

		// OPERATOR OVERLOADS //
		LobMatrix operator *(LobMatrix rhs)
		{
			// VARIABLE INITIALIZATION //
			LobMatrix newMat = LobMatrix((*this).numCols, rhs.numRows);

			// ASSERTS VALID INPUT //
			if((*this).numCols != rhs.numRows)
			{
				std :: cout << "ERROR: ATTEMPTED MATRIX MULTIPLICATION, BUT NUMBER OF "
					<< "COLUMNS ON LEFT MATRIX DOES NOT EQUAL NUMBER OF ROWS ON "
					<< "RIGHT MATRIX!" << std :: endl;
				return newMat;
			}

			for(int x = 0; x < newMat.numCols; x++)
				for(int y = 0; y < newMat.numRows; y++)
					newMat.setPoint(y, x, math :: dot((*this), rhs, y, x));

			// RETURNS MULTIPLIED VALUE //
			return newMat;
		}

		LobMatrix& operator *=(LobMatrix rhs)
		{
			(*this) = (*this) * rhs;
			return (*this);
		}

	private :
		// UTILITY METHODS //


		// PRIVATE ATTRIBUTES //
		std :: vector<float> data;
		unsigned int numRows = 0;
		unsigned int numCols = 0;
};

#endif
#ifndef LOBSTER_INPUT
#define LOBSTER_INPUT 1

// STANDARD INCLUDES //
#include <iostream>
#include <string>
#include <bitset>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <globals.h>
#include <entity.h>
#include <camera.h>
#include <utilities.h>
#include <scene.h>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>

/// <summary> ///
///		This struct controls the state of what buttons are pressed, the state of
///			control keys, and the position/delta-value of the mouse.
/// </summary> ///

struct InputState
{
	// IS A BIT MASK FOR EVERY PRESSED KEY //
	std :: bitset<128> pressedKeys = {0};

	// HOLDS THE CURRENT POSITION OF THE MOUSE //
	int mouseDeltaX = 0; int mouseDeltaY = 0; 
	int mouseX = 0; int mouseY = 0;
	int scroll = 0; bool mmb = false; bool lmb = false; bool rmb = false;

	bool mouseInput = false;

	// BOOLEANS DESCRIBING THE CURRENT VALUE OF CONTROL KEYS //
	bool cntrlPressed = false;
	bool shiftPressed = false;
	bool altPressed = false;
};

namespace inputManagement
{
	///	<summary> ///
	///		Creates an input state pointer and returns it.
	/// </summary> ///

	InputState * createInputState();

	/// <summary> ///
	///		Resets the value of the inptu state. This is used to ensure that
	///			values which relative values (like mouseX/mouseY) are continually
	///			reset so that input processing works as intended.
	/// </summary> ///

	void resetInput(InputState * input);

	/// <summary> ///
	///		Processes user input to manipulate the position and rotation of the
	///			viewport camera.
	///		TODO: Add support for selecting entities, using shortcuts, etc.
	/// </summary> ///

	void processInput(entityID cameraID);
};

#endif
#ifndef LOBSTER_SCRIPT_H
#define LOBSTER_SCRIPT_H

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <string>
#include <vector>

// LOBSTER INCLUDES //
#include <entity.h>
#include <APIUtils.h>

/// <summary> ///
///		This file holds the struct for the "Script" component,
///			which represents a script attached to an entity.
/// </summary> ///

struct Script
{
	std :: string name;
	entityID id;
	pybind11 :: object code;
};

namespace scriptHandler
{
	Script * createScript(std :: string scriptName = "", entityID entRef = 0);
}

#endif
#ifndef LOBSTER_ENGINE_CORE
#define LOBSTER_ENGINE_CORE 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <vendorIncludes.h>

// STRUCT PROTOTYPES //
struct Scene;
struct Pipeline;
struct InputState;

/// <summary> ///
///		This namespace includes value crucial to keeping the engine functional at the most
///			fundamental level. 
/// </summary> ///

namespace globals
{
	/// <summary> ///
	///		If this boolean is ever set to false, turns off the application.
	/// </summary> ///

	extern bool isRunning;
	extern bool isPlaying;

	/// <summary> ///
	/// 	This pointer references the main window.
	/// </summary> ///

	extern SDL_Window * winRef;

	/// <summary> ///
	///		These values dictate the starting width/height of the engine window.
	///			(Defaults to standard HD 16:9 resolution.)
	/// </summary> ///

	extern unsigned short int winWidth;
	extern unsigned short int winHeight;

	/// <summary> ///
	///		This pointer references the OpenGL context.
	/// </summary> ///

	extern void * glRef;

	/// <summary> ///
	///		This field contains the currently actively scene.
	/// </summary> ///

	extern Scene * curSceneRef;

	/// <summary> ///
	///		This field contains a reference to the general input state
	///			of the application. 
	/// </summary> ///

	extern InputState * inputState;

	/// <summary> ///
	///		A collection of all created pipelines within the engine context.
	/// </summary> ///

	extern std :: vector<Pipeline *> pipelineRefs;

	/// <summary> ///
	///		This field holds the time passed since the last time getDeltaTime was
	///			called. getDeltaTime can be found in the appManagement namespace.
	/// </summary> ///

	extern double deltaTime;

	/// <summary> ///
	///		The color that the screen is automatically cleared to at the beginning
	///			of each frame.
	/// </summary> ///

	extern float clearColor[4];
	extern float tempScale;
	extern std :: vector<float> tempRot;
};

namespace editorGlobals
{
	/// <summary> ///
	///		These flags define the baseline behavior for different types of UI
	///			objects. 
	/// </summary> ///

	extern ImGuiWindowFlags 	windowFlags;
	extern ImGuiTreeNodeFlags 	sceneTreeFlags;
	extern ImGuiInputTextFlags	inputTextFlags;

	/// <summary> ///
	///		This bool describes whether or not the user is currently entering key
	///			input to a UI item. If so, it disables camera movement.
	/// </summary> ///

	extern bool keyInput;

	/// <summary> ///
	///		This ID represents the entity currently selected within the scene hiearchy.
	/// </summary> ///

	extern entityID curActiveEntity;
	extern bool entitySelected;

	/// <summary> ///
	///		These values describe the parameters in which the editor frontend is organized.
	/// </summary> ///

	extern float sidePanelWidth;
	extern float sidePanelHeight;

	extern float bottomPanelWidth;
	extern float bottomPanelHeight;

};

#endif
#ifndef LOBSTER_TRANSFORM
#define LOBSTER_TRANSFORM 1

// INCLUDES AND INITIALIZATIONS //

// LOBSTER INCLUDES //
#include <math.h>
#include <globals.h>

// STD INCLUDES //
#include <vector>

// VENDOR INCLUDES //
#include <glm/glm.hpp>
#include <glm/geometric.hpp> 				// glm :: normalize
#include <glm/ext/matrix_transform.hpp> 	// glm :: translate/rotate/scale/lookAt

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/string_cast.hpp>			// glm :: to_string

// STRUCT DEFINITIONS //

/// <summary> ///
///		This struct represents the center point of a mesh in world space coordinates,
///			the rotation of that object centered about its origin point,
///			and the scale of the object.
/// </summary> ///

struct Transform
{
	std :: vector<float> position = { 0.0f, 0.0f, 0.0f };
	std :: vector<float> rotation = { 0.0f, 0.0f, 0.0f };
	std :: vector<float> scale = { 1.0f, 1.0f, 1.0f };
};

namespace transformHandler
{
	/// <summary> ///
	///		Creates and returns a reference to a new transform component.
	/// </summary> ///
	Transform * createTransform();

	Transform * createTransform
	(
		std :: vector<float> inputPosition,
		std :: vector<float> inputRotation,
		std :: vector<float> inputScale
	);

	/// <summary> ///
	///		This function deep copies and returns a Transform component.
	/// </summary> ///

	Transform * duplicateTransform(Transform * base);

	/// <summary> ///
	///		Returns a "worldMatrix" based on the data inside a target transform
	///			component.
	/// </summary> ///
	glm :: mat4x4 getObjectWorldMatrix(Transform * inputTrans);

	/// <summary> ///
	///		These functions get the matrices which typically form the model
	///			world matrix.
	/// </summary> ///
	glm :: mat4x4 getRotateMatrix(Transform * inputTrans);
	glm :: mat4x4 getTranslateMatrix(Transform * inputTrans);
	glm :: mat4x4 getScaleMatrix(Transform * inputTrans);

	/// <summary> ///
	/// 	Modifies the position value of input transform by input delta vector.
	/// </summary> ///

	void translate(Transform * inputTrans, std :: vector<float> deltaVector, bool applyGlobal=false);

	/// <summary> ///
	/// 	Modifies the rotation value of input transform by input delta vector.
	/// </summary> ///

	void rotate(Transform * inputTrans, std :: vector<float> deltaVector);

	/// <summary> ///
	/// 	Modifies the scale value of input transform by input delta vector.
	/// </summary> ///

	void scale(Transform * inputTrans, std :: vector<float> deltaVector);

	/// <summary> ///
	///		Processes orders given from the Python API, which pertain to transform components.
	/// </summary> ///
	void processOrder(std :: string orderName, entityID entID, std :: vector<pybind11 :: object> params);
};

#endif
#ifndef LOBSTER_LIGHT_H
#define LOBSTER_LIGHT_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <vector>

// STRUCT PROTOTYPES //

/// <summary> ///
///		This struct is used to emit light in a scene.
/// </summary> ///

struct Light
{
	/// <summary> ///
	///		This four dimensional vector is used to describe the color
	///			of the emitter's light. The fourth element is used to
	///			describe the brightness of the light.
	/// </summary> ///

	std :: vector<float> color;
};

namespace lightHandler
{
	/// <summary> ///
	///		Creates and returns a pointer to a new light componenet.
	/// </summary> ///
	Light * createLight
	(
		std :: vector<float> lightColor = 
			std :: vector<float> { 1.0f, 1.0f, 1.0f, 0.25f }
	);
};

#endif
#ifndef LOB_TEX_H
#define LOB_TEX_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <string>
#include <iostream>

// VENDOR INCLUDES //
#include <stb_image.h>

/// <summary> ///
///		This struct  defines the dimensions of the image, and holds a reference
///			to the beginning of the texture's pixel data in memory.
/// </summary> ///

struct Texture
{
	unsigned char * pixels;
	unsigned width; unsigned height;
	std :: string name;
};

// FUNCTION DEFINITIONS //
namespace textureHandler
{
	Texture * createTexture(std :: string textureName);
};

#endif
// VENDOR INCLUDES //
#include <stb_image.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#ifndef LOBSTER_MESH
#define LOBSTER_MESH 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <pybind11/pybind11.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <graphics.h>
#include <importantConstants.h>
#include <scene.h>
#include <math.h>
#include <texture.h>

/// <summary> ///
///		This class holds the basic vertex/index/texture and other such attributes
///			so that the object as a whole can be rendered easily by OpenGL.
/// </summary> ///
struct Mesh
{
	std :: string name;
	std :: string texName;
	bool reload = false;

	std :: vector<GLfloat> vertexData = { };
	std :: vector<GLuint> indexData = { };

	GLuint vertexBuffer;
	GLuint indexBuffer;

	unsigned short int vertexCount = 0;
	unsigned short int indexCount = 0;
	GLuint VAO = 0;

	GLuint textureID = 0;	
	SDL_Surface * texSurf;
};

// FUNCTION DEFINITIONS //
namespace meshHandler
{
	/// <summary> ///
	///		This function creates and returns a new mesh component with input vertices,
	///			and assigns it the color of input color struct.
	/// </summary> ///
	Mesh * createMesh
	(
		std :: vector<GLfloat> inputVertices = std :: vector<GLfloat> { }, 
		std :: vector<GLuint> inputIndices = std :: vector<GLuint> { },
		std :: string meshName = "mesh"
	);

	void drawMesh(Mesh * inputMesh);

	Mesh * getMeshFromPLY(std :: string fileName, bool debugPrint = false);

	void printMesh(Mesh * inputMesh);

	// <summary> ///
	///		This function loads a texture image from input file and assigns its
	///			pixel data to the input mesh struct.
	/// </summary> ///

	void setTexture(Mesh * inputMesh, Texture * inputTexture);

	/// <summary> ///
	///		Processes orders given from the Python API, which pertain to Mesh
	///			components.
	/// </summary> ///

	void processOrder(std :: string orderName, entityID entID, std :: vector<pybind11 :: object> params);
};

#endif
#ifndef LOBSTER_CONSTANTS
#define LOBSTER_CONSTANTS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <map>

/// <summary> ///
///		These constants define the maximum entities and componenets for each
///			scene, giving a compile-time constant value for the scene manager to
///			use while creating new scene objects.
/// </summary> ///
#define LOBSTER_MAX_ENTITIES 	256
#define LOBSTER_MAX_COMPONENTS	32
#define LOBSTER_MAX_LIGHTS		32

// ECS ID'S/MASKS TO IMPROVE READABILITY //
typedef unsigned short int  	entityID;
typedef unsigned char 			componentID;
typedef unsigned int 			componentMask;
typedef char *					compPtr;

// DEFINES VERTEX ATTRIBUTES //
#define POSITION_VEC				0
#define COLOR_VEC					1
#define CORD_VEC					2
#define NORMAL_VEC					3

// DEFINES UNIFORMS //
#define UNI_MODEL_WORLD_MATRIX		0
#define UNI_WORLD_VIEW_MATRIX		1
#define UNI_VIEW_PROJ_MATRIX		2
#define UNI_2D_SAMPLER				3
#define UNI_LIGHT_POS				4
#define UNI_LIGHT_COLOR				5

// COMPONENT DEFINTIONS //
#define MESH_COMP_ID				0
#define TRANS_COMP_ID				1
#define CAMERA_COMP_ID				2
#define LIGHT_COMP_ID				3
#define SCRIPT_COMP_ID				4

// THIS VALUE REPRESENTS THAT THE PLAYER HAS REACHED THE END OF ALL //
	// CURRENTLY DEFINED VALUES //
#define CUR_COMP_MAX				5 

// THIS VALUE REPRESENTS A "NULL" VALUE, AND CAN BE //
	// USED FOR ERROR CHECKING //
#define NULL_COMP_ID				LOBSTER_MAX_COMPONENTS + 1

// TOOLS FOR IMGUI //
#define LOB_FILE_NAME_MAX 64

#endif
#ifndef LOBSTER_PIPELINE
#define LOBSTER_PIPELINE 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <iostream>
#include <fstream>
#include <filesystem>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <utilities.h>
#include <globals.h>
#include <importantConstants.h>
#include <vendorIncludes.h>

/// <summary> ///
///		This struct holds an OpenGL program, and containers for the latest loaded shaders
///			of each programmable type of shader.
/// </summary> ///

struct Pipeline
{
	GLuint program;
	GLuint vertShader; std :: string vertShaderName;
	GLuint fragShader; std :: string fragShaderName;
	bool isCompiled = false;
};

namespace graphicManagement
{
	// PUBLIC METHODS //

	/// <summary> ///
	///		Creates and returns a new, fully initialized pipeline instance.
	/// </summary> ///
	Pipeline * createPipeline();

	/// <summary> ///
	///		Loads a shader at input shader path into input target pipeline, and compiles
	//			it as input shader type. The shader can be printed fully in console
	///			if the caller sets input debug bool as true.
	/// </summary> ///
	void loadShader
	// PARAMETERS //
	(
		Pipeline * targetPipeline, GLenum shaderType, const char * shaderPath, 
		bool debugPrint = false
	);

	/// <summary> ///
	///		Attaches the latest loaded shaders to the render pipeline.
	/// </summary> ///
	void compileProgram(Pipeline * targetPipeline);

	/// <summary> ///
	///		Sets program of input target pipeline as the active program.
	/// </summary> ///
	void usePipeline(Pipeline * targetPipeline);

	/// <summary> ///
	///		This function clears the screen a specific color, and prepares the
	///			engine for another render pass.
	/// </summary> ///
	void beginRenderPass();

	/// <summary> ///
	///		Flips the buffers over using a more engine-friendly function
	///			than the lower level SDL2 function.
	/// </summary> ///
	void present();
};

#endif
