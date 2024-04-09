// BASIC FRAGMENT SHADER //
#version 460

// INCLUDES AND INITIALIZATIONS //

// DEFINES CONSTANTS //
vec3 c_ambientAbove	= 	vec3(0.3f, 0.3f, 0.3f);
vec3 c_ambientBelow	= 	vec3(0.1f, 0.1f, 0.1f);
const int c_maxLights = 32;

// DEFINES UNIFORMS //
layout (location = 3) uniform sampler2D 			u_2DSampler;
layout (location = 4) uniform vec3 					u_lightPos[32];
layout (location = 4 + c_maxLights) uniform vec4 	u_lightColor[32];
	// Why in the WORLD does it work like this?

// INPUT ATTRIBUTES //
layout (location = 0) in vec2 in_texCord;
layout (location = 1) in vec4 in_colorVal;
layout (location = 2) in vec3 in_normalVec;
layout (location = 3) in vec3 in_pixelPos;

// OUTPUT ATTRIBUTES //
out vec4 out_colorVal;

void main()
{
	out_colorVal = vec4(0.25f, 0.25f, 0.25f, 1.0f);
}
