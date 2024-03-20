#ifndef LOBSTER_SCRIPT_H
#define LOBSTER_SCRIPT_H

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <string>
#include <vector>

// LOBSTER INCLUDES //
#include <entity.h>

/// <summary> ///
///		This file holds the struct for the "Script" component,
///			which represents a script attached to an entity.
/// </summary> ///

struct Script
{
	std :: string name;
	pybind11 :: object code;
};

namespace scriptHandler
{
	Script * createScript(std :: string scriptName, entityID entRef);
}

#endif
