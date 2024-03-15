// INCLUDES DEFINITION FILE //
#include <script.h>

// FUNCTION IMPLEMENTATIONS //
void scriptManagement :: initializeScript(std :: string name)
{
	std :: cout << "INITIALIZING " << name << std :: endl;
	Script * newScript = new Script;
	newScript -> targetFile = name;
	globals :: scripts.emplace(name, newScript);
	for(auto keyPair : globals :: scripts)
		std :: cout << "SCRIPT NAMED " << keyPair.first << std :: endl;
}

Script * scriptManagement :: getScript(std :: string name)
	{ return globals :: scripts[name]; }

void scriptManagement :: setStart(std :: string inputName, pybind11 :: function inputFunction)
{
	for(auto keyPair : globals :: scripts)
		std :: cout << "(START) SCRIPT NAMED " << keyPair.first << std :: endl;
	std :: cout << "SCRIPT " << inputName << "'S STARTING FUNCTION IS SET!" << std :: endl;	
	globals :: scripts[inputName] -> startFunction = inputFunction; 
}

void scriptManagement :: setUpdate(std :: string inputName, pybind11 :: function inputFunction)
{
	std :: cout << "SCRIPT " << inputName << "'S UPDATE FUNCTION IS SET!" << std :: endl;	
	globals :: scripts[inputName] -> startFunction = inputFunction; 
}
