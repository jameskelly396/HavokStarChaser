// This shader is from the OpenGL Programming Guide, 8th edition, page 378 - 379

// FRAGMENT shader for multiple lights

#version 400

// Point light: isLocal(GL_FALSE), isSpot(GL_FALSE) : need position only
// Spot light: isLocal(GL_FALSE), isSpot(GL_TRUE) : need position + coneDirection + spotCosCutOff + spotExponent
// Direction light: isLocal(GL_TRUE) : Need "halfVector"  (a "fast" form of direction)

struct LightProperties
{
	bool isEnabled;			// GL_FALSE, GL_TRUE
	bool isLocal;			// Only for directional lights
	bool isSpot;
	vec3 ambient;			// Like "color" but a lot less (0.2, 0.2, 0.2)
	vec3 color;				// Tip: make it a plain, boring white light (1.0, 1.0, 1.0)
	vec3 position;			// Point or spot
	vec3 halfVector;			// Only used for directional lights.
								// See code below to see how it's calculated.
	vec3 coneDirection;		// Spot lights
	float spotCosCutoff;	// Spot lights
	float spotExponent;		// Spot lights
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

// the set of lights to apply, per invocation of this shader
const int MaxLights = 10;
uniform LightProperties Lights[MaxLights];

uniform float Shininess;
uniform float Strength;
uniform vec3 EyeDirection;

// Added to blend the textures 
uniform float textureMix;

// ADDED on October 17, 2014
uniform sampler2D texture_0;
uniform sampler2D texture_1;

in vec4 ex_Position;
in vec4 ex_Color;
in vec3 ex_Normal;			// Note Normal is a vec3
in vec4 ex_DebugObjectColourOverride;	// If the w is != 0, this colour is used (lighthing is ignored)

// ADDED on October 17, 2014
in vec4 ex_TexUV_x2;


out vec4 out_Color;		// If you don't say otherwise, this is the pixel colour

// FRAGMENT

void main(void)
{
	vec3 scatteredLight = vec3(0.0);		// or, to a global ambient light
	vec3 reflectedLight = vec3(0.0);
	
	vec4 surfaceColor = ex_Color;
	
	vec4 textColour_0 = texture( texture_0, ex_TexUV_x2.xy );	 // sampler2D	
	vec4 textColour_1 = texture( texture_1, ex_TexUV_x2.xy );

	//surfaceColor = textColour_0 + textColour_1;		// Replace colour with texture "colour"
	//surfaceColor = textColour_0 * textColour_1;		// Replace colour with texture "colour"
	surfaceColor = mix(textColour_0, textColour_1, textureMix );
	
	// Are we using the debug colour? 
	if ( ex_DebugObjectColourOverride.w != 0.0f )
	{
		// use the debug colour override
		out_Color = vec4( ex_DebugObjectColourOverride.xyz, 1.0f );
	}
	else
	{	// Do the full lighthing and texture calculation

		// loop over all the lights
		for ( int light = 0; light < MaxLights; ++light )
		{ 
			if ( ! Lights[light].isEnabled )
				continue;
			
			vec3 halfVector;
			vec3 lightDirection = Lights[light].position;
			float attenuation = 1.0;
		
			// for local lights, compute per-fragment direction,
			// half-vector, and attenuation
			if ( Lights[light].isLocal )			// aks Directional lighting
			{
				lightDirection = lightDirection - vec3(ex_Position);
				float lightDistance = length(lightDirection);
				lightDirection = lightDirection / lightDistance;
			
				attenuation = 1.0f / 
					(Lights[light].constantAttenuation
					+ Lights[light].linearAttenuation * lightDistance
					+ Lights[light].quadraticAttenuation * lightDistance * lightDistance);
			
				if (Lights[light].isSpot)
				{
					float spotCos = dot(lightDirection, 
										-Lights[light].coneDirection);
				
					if ( spotCos < Lights[light].spotCosCutoff)
					{
						attenuation = 0.0f;
					}
					else
					{
						attenuation *= pow(spotCos, 
										   Lights[light].spotExponent);
					}
				} // if (Lights[light].isSpot)
			
				halfVector = normalize( lightDirection + EyeDirection );
			}
			else
			{
				halfVector = Lights[light].halfVector;
			}
		
			float diffuse = max( 0.0, dot(ex_Normal, lightDirection) );
			float specular = max( 0.0, dot(ex_Normal, halfVector) );
		
			if (diffuse <= 0.0)
			{
				specular = 0.0;
			}
			else
			{
				specular = pow(specular, Shininess) * Strength;
			}
		
			// Accumulate all the lights effects
			scatteredLight += Lights[light].ambient * attenuation + 
							  Lights[light].color * diffuse * attenuation;
		
			reflectedLight += Lights[light].color * specular * attenuation;
		}// for ( int light = 0, ...
	
		vec3 rgb = min(surfaceColor.rgb * scatteredLight + reflectedLight, vec3(1.0) );
	
		out_Color = vec4(rgb, surfaceColor.a);

	}// if ( ex_DebugObjectColourOverride.w <> 0.0f )
	
	// For debugging
	//  out_Color = ex_Color; 
	//out_Color = vec4( ex_Normal.xyz, 1.0f );


	//out_Color *= 0.01f;	// Drop the colours to almost black
	//out_Color.xy += ex_TexUV_x2.xy;

	//out_Color.xyz += textColour_0.xyz;
	//out_Color.xyz += surfaceColor.xyz;
}
