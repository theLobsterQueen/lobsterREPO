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
