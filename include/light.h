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
