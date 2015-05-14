// This is taken from the OpenGL Programming Guide, 8th edition, page 369

#version 330 core

uniform vec3 Ambient;
uniform float Shininess;

uniform vec3 EyeDirection;

uniform vec3 LightColor;
uniform vec3 LightPosition;		// Location of the light in eye space
uniform float ConstantAttenuation;	
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;
uniform float Strength; 

in vec4 Color;
in vec3 Normal;
in vec4 Position;

void main()
{
	// Find the direction and distance of the light, 
	// which changes fragment to fragment for a local light
	vec3 lightDirection = LightPosition - vec3(Position);
	float lightDistance = length( lightDirection );

	// Normalize the light direction vector, so
	// that dot products give cosines
	lightDirection = lightDirection / lightDistance;

	// model how much light is available for this fragment
	float attenuation = 1.0 / 
			( ConstantAttenuation + 
			    LinearAttenuation * lightDistance + 
			 QuadraticAttenuation * lightDistance * lightDistance );

	// the direction of maxiumum hightlight also changes per fragment
	vec3 halfVector = normalize( lightDirection + EyeDirection );

	float diffuse = max( 0.0, dot( Normal, lightDirection ) );
	float specular = max( 0.0, dot( Normal, halfVector ) );

	if ( diffuse == 0.0 )
	{
		specular = 0.0;
	}
	else
	{
		specular = pow( specular, Shininess ) * Strength;
	}

	vec3 scatteredLight = Ambient + LightColor * diffuse * attenuation;
	vec3 reflectedLight = LightColor * specular * attenuation;
	vec3 rgb = min( Color.rgb * scatteredLight + reflectedLight, vec3(1.0) );

	FragColor = vec4( rgb, Color.a );
}
