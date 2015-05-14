#version 400

// in and out mean vertex buffer
layout(location=0) in vec4 in_Position;  
layout(location=1) in vec4 in_Color;	

out vec4 ex_Color;
out vec4 VertPosition;		

uniform mat4 ModelMatrix;    
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec4 objectColour;		
uniform vec4 LightPosition;
uniform vec4 LightColour; 
uniform float attenuation;


void main(void)
{
	mat4 OneMatrixToRuleThemAll = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// gl_Position is a built-in variable
	gl_Position = OneMatrixToRuleThemAll * in_Position;

	VertPosition = ModelMatrix * in_Position;			// ADDED September 30, 2014
	ex_Color = objectColour;
}