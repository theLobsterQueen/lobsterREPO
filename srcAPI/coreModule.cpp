// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <entity.h>

// MAKES THE SCRIPT MAP OPAQUE //
PYBIND11_MAKE_OPAQUE(scriptContainer);

// FUNCTIONS FOR HANDLING SCRIPT MANAGEMENT //
void addScript(scriptContainer& scripts, std :: string scriptName)
	{ scripts[scriptName] = { }; }
void addScriptFunc
	(scriptContainer& scripts, std :: string scriptName, std :: string funcName, pybind11 :: function func)
{ 
	scripts[scriptName][funcName] = func; 
}

void printScriptContainer(scriptContainer scripts)
{
	std :: cout << "Print out script container!" << std :: endl;
	for(auto pair : scripts)
	{
		for(auto secondPair : pair.second)
			std :: cout << "\t\t" << secondPair.first << std :: endl;
	}
}

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
