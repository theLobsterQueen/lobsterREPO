// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>

// LOBSTER INCLUDES //
#include <entity.h>

// BINDS MODULE //
PYBIND11_MODULE(_entityapi, m)
{
	pybind11 :: class_<Entity>(m, "Entity")
		.def_readwrite("name", &Entity :: name)
		.def_readwrite("ID", &Entity :: ID);
}
