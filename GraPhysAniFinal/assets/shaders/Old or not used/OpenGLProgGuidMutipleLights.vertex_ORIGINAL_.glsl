// This shader is from the OpenGL Programming Guide, 8th edition, pg 377-378

// VERTEX shader

#version 330 core

uniform mat4 ModelMatrix;   
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

// Note This is different from the book:
// - We are passing the 3 separate matrix values, then combining in shader
// - We aren't passing the NormalMatrix
//uniform mat4 MVPMatrix;			// MVP Model, View, Projection 
//uniform mat4 MVMatrix;			// MV  Model, View only
//uniform mat3 NormalMatrix;		// Note we're not passing this

layout(location=0) in vec4 VertexPosition;
layout(location=1) in vec4 VertexNormal;		// Note this is vec3 in the book
layout(location=2) in vec4 VertexColor;


out vec4 Position;
out vec3 Normal;			// Note Normal is a vec3
out vec4 Color;

void main()
{
	// Had been added to the shader
	mat4 MVMatrix = ViewMatrix * ModelMatrix;
	mat4 MVPMatrix = ProjectionMatrix * MVMatrix;

	Color = VertexColor;

	// This line is slightly different in the book, as VertexNormal is vec3
	// From book: Normal = normalize(NormalMatrix * VertexNormal );
	Normal = vec3( VertexNormal );

	Position = MVMatrix * VertexPosition;
	gl_Position = MVPMatrix * VertexPosition;
}
