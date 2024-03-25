// BASIC 3D VERTEX SHADER //
#version 460

// DEFINES UNIFORMS //
layout (location = 0) uniform mat4 u_modelWorldMatrix;
layout (location = 1) uniform mat4 u_worldViewMatrix;
layout (location = 2) uniform mat4 u_viewProjMatrix;

// INPUT ATTRIBUTES //
layout (location = 0) in vec3 in_vertexPos;
layout (location = 1) in vec4 in_colorVal;
layout (location = 2) in vec2 in_texCord;
layout (location = 3) in vec3 in_normalVec;

// OUTPUT ATTRIBUTES //
layout (location = 0) out vec2 out_texCord;
layout (location = 1) out vec4 out_colorVal;
layout (location = 2) out vec3 out_normalVec;
layout (location = 3) out vec3 out_pixelPos;

void main()
{

	// TRANSFORMS VERTEX ATTRIBUTE INTO 4D OBJECT //
	vec4 vertexPos = vec4(in_vertexPos, 1.0);
	vertexPos *= (u_modelWorldMatrix * u_worldViewMatrix * u_viewProjMatrix);

	// SETS OUTPUT DATA //
	gl_Position = vertexPos; 

	// SETS OUTPUT ATTRIBUTE DATA //
	out_texCord = in_texCord;
	out_colorVal = in_colorVal;
	out_normalVec = (vec4(in_normalVec, 0.0f) * u_modelWorldMatrix).xyz;
	out_pixelPos = (vec4(in_vertexPos.xyz, 1.0f) * u_modelWorldMatrix).xyz;
}
