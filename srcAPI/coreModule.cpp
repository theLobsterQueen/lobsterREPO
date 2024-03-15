// INCLUDES DEFINITION FILE //
#include <coreModule.h>

// FUNCTION IMPLEMENTATIONS //
void APIStart()
{
	std :: cout << "Script started!" << std :: endl;
}

void APIUpdate(float deltaTime)
{
	std :: cout << "Script updating! Current delta time is: " << deltaTime << std :: endl;
}

// CREATES THE CORE MODULE //
PYBIND11_MODULE(coremodule, m)
{
	// MODULE DOCSTRING //
	m.doc() = std :: string
	(
	 	std :: string("This is the core module for the Lobster Engine Python Scripting API.\n")
		+ std :: string("\tIt has all key functions necesarry for communicating with the fundamental\n")
		+ std :: string("\tdata found in the Lobster Engine backend.")
	).c_str();

	// FUNCTION IMPLEMENTATION//
	m.def("initialize", &(scriptManagement :: initializeScript), 
		std :: string("Initializes the script and creates data for use on the engine backend.").c_str());
	m.def("set_start", &(scriptManagement :: setStart), "Sets the start function of the script.");
	m.def("set_update", &(scriptManagement :: setUpdate), "Sets the update function of the script.");
}
