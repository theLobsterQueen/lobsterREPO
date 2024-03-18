#ifndef LOBSTER_API_CORE
#define LOBSTER_API_CORE 1

/// <summary> ///
///		This file defines and configures structs, classes and functions
///			which will be binded into the Python scripting API. These are usually simplified
///			interpretations of core engine components, such as component structs. 
///		Note that some components are merely included in this file, and not re-defined in
///			any way: those structs will be binded as-is in the coreBindings.cpp file.
///		There are also some functions that do not have a C++ analog, and rather are tools
///			specifically designed for users in the Python API. 
/// </summary> ///

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <globals.h>

#endif
