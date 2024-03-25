// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <light.h>
#include <APIUtils.h>

// FUNCTIONS FOR THE COMPONENT //
void setColor(Light& self, std :: vector<float> newVec)
	{ self.color = newVec; }
std :: vector<float> getColor(Light& self)
	{ return self.color; }

// BINDS THE COMPONENT //
PYBIND11_MODULE(_lightapi, m)
{
	pybind11 :: class_<Light>(m, "Light")
		// BINDS FUNCTIONS //
		.def(pybind11 :: init<>())

		// GETTERS AND SETTERS FOR TRANSFORM DATA //
		.def("set_color", &setColor)
		.def("get_color", &getColor);
}
