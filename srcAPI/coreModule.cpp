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

// CREATES THE CORE MODULE //
PYBIND11_MODULE(_coremodule, m)
{
	// MODULE DOCSTRING //
	m.doc() = std :: string
	(
	 	std :: string("This is the core module for the Lobster Engine Python Scripting API.\n")
		+ std :: string("\tIt has all key functions necesarry for communicating with the fundamental\n")
		+ std :: string("\tdata found in the Lobster Engine backend.")
	).c_str();
}
