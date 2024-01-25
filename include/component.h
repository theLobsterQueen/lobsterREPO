#ifndef LOBSTER_COMPONENTS
#define LOBSTER_COMPONENTS 1

// INCLUDES AND INITIALIZATIONS //
#include <importantConstants.h>

namespace componentManagement
{
	// FUNCTION DEFINITIONS //
	static unsigned char componentCounter = 0;

	/// <summary> ///
	///		This file returns an ID that references a specific component type.
	/// </summary> ///
	template <class T>
	componentID getID()
	{
		static componentID id = componentCounter++;
		return id;
	}
};

#endif
