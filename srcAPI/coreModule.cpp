// INCLUDES DEFINITION FILE //
#include <coreModule.h>

// STATIC FILE-GLOBAL VARIABLES //
const static std :: vector<std :: string> watchedMethods = WATCHED_METHODS;

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
		std :: cout << "\tScript " << pair.first << " has: " << std :: endl;
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

	pybind11 :: class_<scriptContainer>(m, "ScriptContainer")
		.def("add_script", &addScript)
		.def("add_script_func", &addScriptFunc)
 		.def("out", &printScriptContainer);

	// VARIABLE EXPORTS //
	m.attr("scripts") = new scriptContainer;
	m.attr("watched_methods") = watchedMethods;
}
