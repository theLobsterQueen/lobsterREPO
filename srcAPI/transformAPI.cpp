// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <vector>

// LOBSTER INCLUDES //
#include <transform.h>

// DEFINES FUNCTIONS FOR THIS MODULE //
/*
	void rotateTransform(Transform inputTransform, std :: vector<float> rotDelta)
		{ transformHandler :: rotate(&inputTransform, rotDelta); }
*/

// BINDS THE MODULE //
PYBIND11_MODULE(_transformapi, m)
{
	pybind11 :: class_<Transform>(m, "Transform")
		.def_readwrite("position", &Transform :: position)
		.def_readwrite("rotation", &Transform :: rotation)
		.def_readwrite("scale", &Transform :: scale);
}
