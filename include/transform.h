#ifndef LOBSTER_TRANSFORM
#define LOBSTER_TRANSFORM 1

struct Transform
{
	/// <summary> ///
	///		This field describes whether or not the attached entity is "centered."
	///			If an entity is centered it is drawn without regarding player position
	///			offset.
	/// </summary> ///
	bool isCentered = false;
};

#endif
