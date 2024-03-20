// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <entity.h>
#include <scene.h>
#include <APIUtils.h>

// FUNCTIONS FOR THE COMPONENT //
void rotate(Transform& self, std :: vector<float> deltaVec)
{
	for(unsigned char i = 0; i < 3; i++)
		self.rotation[i] += deltaVec[i];
}

// BINDS THE COMPONENT //
PYBIND11_MODULE(_transformapi, m)
{
	pybind11 :: class_<Transform>(m, "Transform")
		// BINDS FUNCTIONS //
		.def("rotate", &rotate)

		// BINDS ATTRIBUTES //
		.def_readwrite("position", &Transform :: position)
		.def_readwrite("rotation", &Transform :: rotation)
		.def_readwrite("scale", &Transform :: scale);
}
