#ifndef LOBSTER_CAMERA_H
#define LOBSTER_CAMERA_H 1

// INCLUDES AND INIITALIZATIONS //

// LOBSTER INCLUDES //
#include <graphics.h>
#include <math.h>
#include <cmath>
#include <transform.h>

// VENDOR INCLUDES //
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp> 	// glm :: perspective
#include <glm/ext/matrix_transform.hpp> 	// glm :: translate/rotate/scale/lookAt

// PROTOTYPE STRUCTS //
struct Transform;

/// <summary> ///
///		This component is used to render a view of a scene from a specific point
///			to a specific target.
///		(TODO) Make the camera able to render to targets other than the main window.
/// </summary> ///

struct Camera
{
	/// <summary> ///
	///		The aspect ratio of the target viewport.
	/// </summary> ///

	float aspectRatio;

	/// <summary> ///
	///		This horizontal and vertical field of view's for the camera.
	///			A larger FOV will reduce the size of all objects in the screen,
	///			but will allow the viewer to see more of the scene at any given
	///			time.
	///
	///		The horizontal field of view can be set manually, but the 
	///			vertically field of view is automatically determined by the
	///			value of the horizontal field of view and the aspect ratio
	///			of the target viewport.
	/// </summary> ///

	float FOV;

	/// <summary> ///
	///		The two clipping distance values for the camera.
	/// </summary> ///

	float near = 0.1f;
	float far = 100.0f;

	/// <summary> ///
	///		Unit vectors used for calculating the view matrix of the camera.
	/// </summary> ///

	std :: vector<float> look = { 0.0f, 0.0f, -1.0f };
	std :: vector<float> up = { 0.0f, 1.0f, 0.0f };
	std :: vector<float> right = { 1.0f, 0.0f, 0.0f };

	/// <summary> ///
	///		The base pipeline this camera renders with (this can be changed
	///			if a mesh uses a special pipeline.
	/// </summary> ///

	Pipeline * curPipelineRef;
};

namespace cameraHandler
{
	/// <summary> ///
	///		Creates a new camera component, with a FOV of inputFOV value.
	///			When rendered to, it will render objects using input pipeline.
	/// </summary> ///

	Camera * createCamera
		(float inputAspect = 0.0f, Pipeline * inputPipeline = nullptr);

	/// <summary> ///
	///		Returns the view matrix for the camera when called. Can be used in
	///			combination with the projection matrix to render dynamic
	///			3D scenes.
	/// </summary> ///
	
	glm :: mat4x4 getWorldViewMatrix(Camera * camera, Transform * camTrans);

	/// <summary> ///
	///		Zooms the camera in/out by decreasing/increasing the camera's FOV.
	/// </summary> ///

	void zoom(Camera * camera, float zoomDelta);
};

#endif
