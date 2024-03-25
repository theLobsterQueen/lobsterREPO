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
#define NULL_COMP_ID				LOBSTER_MAX_COMPONENTS + 1

// TOOLS FOR IMGUI //
#define LOB_FILE_NAME_MAX 64

#endif
