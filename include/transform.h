#ifndef LOBSTER_TRANSFORM
#define LOBSTER_TRANSFORM 1

// INCLUDES AND INITIALIZATIONS //

// LOBSTER INCLUDES //
#include <math.h>

// STD INCLUDES //
#include <vector>

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
	///		Returns a "worldMatrix" based on the data inside a target transform
	///			component.
	/// </summary> ///
	LobMatrix getWorldMat(Transform * inputTrans);

	/// <summary> ///
	/// 	Modifies the position value of input transform by input delta vector.
	/// </summary> ///

	void translate(Transform * inputTrans, std :: vector<float> deltaVector);

	/// <summary> ///
	/// 	Modifies the rotation value of input transform by input delta vector.
	/// </summary> ///

	void rotate(Transform * inputTrans, std :: vector<float> deltaVector);

	/// <summary> ///
	/// 	Modifies the scale value of input transform by input delta vector.
	/// </summary> ///

	void scale(Transform * inputTrans, std :: vector<float> deltaVector);
};

#endif
