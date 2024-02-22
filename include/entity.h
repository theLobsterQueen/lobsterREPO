#ifndef LOBSTER_ENTITY
#define LOBSTER_ENTITY 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <importantConstants.h>
#include <vector>
#include <bitset>
#include <string>

// STRUCT PROTOTYPES //
struct Scene;

/// <summary> ///
///		This struct is the core to how the engine operates: an entity holds a key
///			and a bitmask which states whether or not it is holding a 
///			certain component of some type in each bit of the mask.
/// </summary> ///
struct Entity
{
	entityID ID;
	componentMask mask = 0;
	std :: string name;
};

#endif
