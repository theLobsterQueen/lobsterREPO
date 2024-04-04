#ifndef LOBSTER_COLLIDER_H
#define LOBSTER_COLLIDER_H 1

// INCLUDES AND INITIALIZATIONS //

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <globals.h>
#include <transform.h>

/// <summary> ///
///		This component is used to calculate the collision state of the entity
///			the component is attached to. It also generates events that can be
///			interpreted by the Scripting API.
/// </summary> ///

struct Collider
{
	// THE WIDTH AND HEIGHT VALUES OF THE COLLIDER //
		// THESE VALUES ARE DERIVED AS DISTANCES FROM THE TRANSFORM POSITION OF THE //
		// ENTITY THIS COMPONENT IS ATTACHED TO //
	// DESCRIPTORS LIKE "LEFT_WIDTH" AND "TOP_HEIGHT" ARE RELATIVE TO THE "FORWARD" VECTOR OF THE //
		// ENTITY THIS COMPONENT IS ATTACHED TO. THE "FORWARD" VECTOR IS THE LOCAL POSITIVE Z DIRECTION //
	float lWidth; float rWidth;
	float uHeight; float dHeight;
	float fDepth; float bDepth;
};

namespace colliderHandler
{
	/// <summary> ///
	///		Creates and returns a collider.
	/// </summary> ///

	Collider * createCollider
		(float lIn, float rIn, float uIn, float dIn, float fIn, float bIn);

	/// <summary> ///
	///		Checks whether two colliders are colliding. Returns a bool that represents their
	///			collision state.
	/// </summary> ///

	bool checkCollision(entityID ent1, entityID ent2);
};


#endif
