// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <APIUtils.h>

// FUNCTIONS FOR THE COMPONENT //
void changeMesh(Mesh& self, std :: string newName)
	{ self.reload = true; self.name = newName; }
void changeTex(Mesh& self, std :: string newName)
	{ self.reload = true; self.texName = newName; }
void disableReload(Mesh& self)
	{ self.reload = false; }

// BINDS THE COMPONENT //
PYBIND11_MODULE(_meshapi, m)
{
	pybind11 :: class_<Mesh>(m, "Mesh")
		// BASE CONSTRUCTOR //
		.def(pybind11 :: init<>())

		// BINDS FUNCTIONS //
		.def("disable_reload", &disableReload)
		.def("change_mesh", &changeMesh)
		.def("change_tex", &changeTex);
}
