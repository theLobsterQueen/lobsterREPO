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

// FUNCTIONS FOR THE COMPONENT //
std :: vector<float> getPosition(Transform& self)
	{ return self.position; }
void setPosition(Transform& self, std :: vector<float> newVec)
	{ self.position = newVec; }
void translate(Transform& self, std :: vector<float> deltaVec)
{
	for(unsigned char i = 0; i < 3; i++)
		self.position[i] += deltaVec[i];
}

std :: vector<float> getRotation(Transform& self)
	{ return self.rotation; }
void setRotation(Transform& self, std :: vector<float> newVec)
	{ self.rotation = newVec; }
void rotate(Transform& self, std :: vector<float> deltaVec)
{
	for(int i = 0; i < 3; i++)
	{
		float value = self.rotation[i] + deltaVec[i];
		while(value > 360)
			value -= 360;
		while(value < -360)
			value += 360;
		self.rotation[i] = value;
	}
}

std :: vector<float> getScale(Transform& self)
	{ return self.scale; }
void setScale(Transform& self, std :: vector<float> newVec)
	{ self.scale = newVec; }
void scale(Transform& self, std :: vector<float> deltaVec)
{
	for(unsigned char i = 0; i < 3; i++)
		self.scale[i] *= deltaVec[i];
}

// BINDS THE COMPONENT //
PYBIND11_MODULE(_transformapi, m)
{
	pybind11 :: class_<Transform>(m, "Transform")
		// BASE CONSTRUCTOR //
		.def(pybind11 :: init<>())

		// BINDS FUNCTIONS //

		// GETTERS AND SETTERS FOR TRANSFORM DATA //
		.def("get_position", &getPosition)
		.def("set_position", &setPosition)
		.def("translate", &translate)
		
		.def("get_rotation", &getRotation)
		.def("set_rotation", &setRotation)
		.def("rotate", &rotate)

		.def("set_scale", &setScale)
		.def("get_scale", &getScale)
		.def("scale", &scale);
}
