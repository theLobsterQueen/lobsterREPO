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
	// SAMPLES TEXTURE COLOR AND BLENDS WITH VERTEX COLOR //
	vec4 texColor = texture(u_2DSampler, in_texCord);
	vec3 colorVal = in_colorVal.xyz * (1 - ((texColor.r + texColor.g + texColor.b) / 3));

	// DETERMINES BASE COLOR FROM VERTEX COLOR AND TEXTURE ALPHA //
	vec3 baseColor = texColor.xyz + colorVal;

	// FINDS NORMAL VECTOR FOR PIXEL //
	vec3 N = normalize(in_normalVec); 

	// APPLIES BASIC AMBIENT LIGHTING //
	float mappedY = 0.5f * (N.y + 1.0f);
	vec3 lightColor = mix(c_ambientBelow, c_ambientAbove, mappedY);

	// APPLIES DIFFUSE LIGHTING //
	for(int i = 0; i < c_maxLights; i++)
	{
		// FIND LIGHT VECTOR //
		vec3 L = normalize(u_lightPos[i] - in_pixelPos); // NORMALIZED LIGHT VECTOR //

		// FINDS THE DOT PRODUCT BETWEEN THE NORMAL AND LIGHT VECTOR //
			// THIS VALUE CAN BE USED TO DETERMINE HOW "ILLUMINATED" THE POINT SHOULD BE //
		float dp = dot(N, L);
		dp = clamp(dp, 0.0f, 1.0f);

		lightColor += (u_lightColor[i].xyz * u_lightColor[i].w) * dp;
	}

	// OUTPUTS TO PIXEL //
	out_colorVal = vec4(baseColor * lightColor, 1.0f);
}
