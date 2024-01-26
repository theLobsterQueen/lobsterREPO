#ifndef LOBSTER_CONSTANTS
#define LOBSTER_CONSTANTS 1

// INCLUDES AND INITIALIZATIONS //

/// <summary> ///
///		These constants define the maximum entities and componenets for each
///			scene, giving a compile-time constant value for the scene manager to
///			use while creating new scene objects.
/// </summary> ///
#define LOBSTER_MAX_ENTITIES 	256
#define LOBSTER_MAX_COMPONENTS	32

// ECS ID'S/MASKS TO IMPROVE READABILITY //
typedef unsigned short int  	entityID;
typedef unsigned char 			componentID;
typedef unsigned int 			componentMask;
typedef char *					compPtr;

// DEFINES VERTEX ATTRIBUTES //
#define POSITION_VEC				0
#define COLOR_VEC					1

// DEFINES UNIFORMS //
#define PLAYER_POS_UNIFORM			0
#define IS_CENTERED_UNIFORM			1

#endif
