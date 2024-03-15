#ifndef LOB_SCRIPT_H
#define LOB_SCRIPT_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <map>

// LOBSTER INCLUDES //
#include <globals.h>

// STRUCT DEFINITIONS //

/// <summary> ///
///		This struct holds pointers to python code, and can be executed on command from
///			the C++ engine backend.
/// </summary> ///

struct Script
{
	std :: string targetFile;
	pybind11 :: function startFunction;
	pybind11 :: function updateFunction;
};

// FUNCTION DEFINITIONS //

namespace scriptManagement
{
	/// <summary> ///
	///		Creates a new script and appends it to the static script map.
	/// </summary> ///

	void initializeScript(std :: string name);

	/// <summary> ///
	///		Returns a script of input name if it exists, or creates one of that
	///			name and returns a pointer to the caller.
	/// </summary> ///

	Script * getScript(std :: string name);

	/// <summary> ///
	///		This function sets the "start" function reference in input script.
	/// </summary> ///

	void setStart(std :: string inputName, pybind11 :: function inputFunction);

	/// <summary> ///
	///		This function sets the "update" function reference in input script.
	/// </summary> ///

	void setUpdate(std :: string inputName, pybind11 :: function inputFunction);
};

#endif
