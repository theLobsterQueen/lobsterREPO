// INCLUDES DEFINITION FILE //
#include <camera.h>

// FUNCTION IMPLEMENTATION //
Camera * cameraHandler :: createCamera(float inputAspect, Pipeline * inputPipeline)
{
	Camera * newCamera = new Camera;

	// IF INPUT ASPECT RATIO IS NULL, DETERMINES INPUT ASPECT RATIO OF VIEWPORT //
	if(inputAspect != 0.0f)
		newCamera -> aspectRatio = inputAspect;
	else
		newCamera -> aspectRatio = ((float) (globals :: winWidth)) / ((float) (globals :: winHeight));
	newCamera -> FOV = 70;

	// IF INPUT PIPELINE IS NULLPTR, CREATES AN EMPTY ONE TO BE USED //
	if(inputPipeline == nullptr)
		inputPipeline = graphicManagement :: createPipeline();
	newCamera -> curPipelineRef = inputPipeline;

	return newCamera;
}

void cameraHandler :: zoom(Camera * camera, float zoomDelta)
{
	// (TODO) REDO THIS VALUE: THE LAST IMPLEMENTATION WAS NOT PROPERLY CONFIGURED //
		// IT RESULTED IN MASSIVE PERSPECTIVE SHEARING //
}

glm :: mat4x4 cameraHandler :: getWorldViewMatrix(Camera * camera, Transform * transform)
{
	glm :: vec3 up(0.0f, -1.0f, 0.0f);
	glm :: vec3 look(0.0f, 0.0f, -1.0f);
	look = glm :: vec4(look[0], look[1], look[2], 1.0f) * transformHandler :: getRotateMatrix(transform);
	glm :: vec3 right = glm :: cross(up, look);
	up = glm :: cross(right, look);
	glm :: vec3 eye(transform -> position[0], transform -> position[1], transform -> position [2]);
	return glm :: transpose(glm :: mat4x4
	(
		right[0], right[1], right[2], glm :: dot(-eye, right), 
		up[0], up[1], up[2], glm :: dot(-eye, up), 
		-look[0], -look[1], -look[2], glm :: dot(eye, look),
		0.0f, 0.0f, 0.0f, 1.0f
	));
}

