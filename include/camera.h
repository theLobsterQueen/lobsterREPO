#ifndef LOBSTER_CAMERA
#define LOBSTER_CAMERA_H 1

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
	float hozFOV;
	float verFOV;
};

#endif
