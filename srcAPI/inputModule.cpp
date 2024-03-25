// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <input.h>
#include <APIUtils.h>

// FUNCTION IMPLEMENTATIONS //
bool isKeyPressed(InputState self, char inputKey)
	{ return self.pressedKeys[inputKey]; }

// CREATES THE CORE MODULE //
PYBIND11_MODULE(_inputmodule, m)
{
	// MODULE DOCSTRING //
	m.doc() = std :: string
	(
	 	std :: string("This module supplies basic input support for the Lobster Engine ")
		+ std :: string("scripting engine.")
	).c_str();

	// CLASS DECLARATIONS //
	pybind11 :: class_<InputState>(m, "InputState")
		.def("priv_is_key_pressed", &isKeyPressed);
}
