#version 400

in vec4 ex_Color;
in vec4 VertPosition;	// Added on September 30, 2014
out vec4 out_Color;		// If you don't say otherwise, this is the pixel colour

// Added September 30, 2014
uniform vec4 LightPosition;
uniform vec4 LightColour; 
uniform float attenuation;
// FRAGMENT

void main(void)
{
	vec3 lightDirection = vec3(LightPosition) - vec3(VertPosition);
	float lightDistance = abs(length( lightDirection ));

	//out_Color = ex_Color * LightColour * attenuation * lightDistance;
	lightDistance /= 10.0f;
	out_Color = vec4( lightDistance, lightDistance, lightDistance, 1.0f );

	// HACK until we get the actual lighting in here
	out_Color = ex_Color;
}