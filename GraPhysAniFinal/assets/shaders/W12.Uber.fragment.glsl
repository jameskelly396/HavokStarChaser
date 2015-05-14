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

uniform samplerCube skyMapTexture;

// For shadow mapping
uniform sampler2DShadow shadow_tex;

uniform bool bIsSkyMap;
uniform bool bIsParticle;
uniform bool bUseDebugColour;
uniform vec4 debugObjectColour;
uniform bool bGenShadowFromLight;	// For shadow generation

in vec4 ex_Position;
in vec4 ex_Color;
in vec3 ex_Normal;			// Note Normal is a vec3
in vec4 ex_ShadowCoord;

// ADDED on October 17, 2014
in vec4 ex_TexUV_x2;


out vec4 final_outColour;		// If you don't say otherwise, this is the pixel colour


void calcLightContrib( inout vec4 outColour, in vec4 surfaceColour, in vec4 vertWorldPosition, in vec3 vertNormal )
{
	vec3 scatteredLight = vec3(0.0);		// or, to a global ambient light
	vec3 reflectedLight = vec3(0.0);

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
	} // for ( int light = 0, ...

	vec3 rgb = min(surfaceColour.rgb * scatteredLight + reflectedLight, vec3(1.0) );	//vec3 rgb = vec3(1.0f, 1.0f, 1.0f);

	outColour = vec4(rgb, surfaceColour.a);
}

// FRAGMENT

void main(void)
{

	
	vec4 surfaceColour = ex_Color;
	
	vec4 textColour_0 = texture( texture_0, ex_TexUV_x2.xy );	 // sampler2D	
	vec4 textColour_1 = texture( texture_1, ex_TexUV_x2.xy );

	//surfaceColour = textColour_0 + textColour_1;		// Replace colour with texture "colour"
	//surfaceColour = textColour_0 * textColour_1;		// Replace colour with texture "colour"
	surfaceColour = mix(textColour_0, textColour_1, textureMix );

	

	// Generating shadow map...?
	if ( bGenShadowFromLight )
	{
		// Output only the depth
		final_outColour = vec4(gl_FragCoord.z);
		// and that's it
	}	
	else if ( bIsParticle )
	{
		// Only use the second texture, which is the flame in our case
		surfaceColour = textColour_1;

		// Supah cool transparency to come...
		final_outColour.a = 5.0f;	
	}
	else if ( bIsSkyMap )	// SKYMAP STUFF
	{
		// Sky map is ONLY contribution from the cubemap texture
		final_outColour = texture( skyMapTexture, ex_Normal.xyz );
		// Slightly brighten the image (because the room is so bright, yo!)
		float brighten = 1.0f;
		float gammaAdjust = 0.0f;
		final_outColour += vec4( gammaAdjust, gammaAdjust, gammaAdjust, 0.0f );
		final_outColour *= vec4( brighten, brighten, brighten, 1.0f );
		//final_outColour = normalize(final_outColour);
		// no transparency
		final_outColour.a = 1.0f;
	}
	// Are we using the debug colour? 
	else if ( bUseDebugColour )
	{
		// use the debug colour override
		//final_outColour = vec4( debugObjectColour, 1.0f );
		final_outColour = debugObjectColour;
		//final_outColour.a = 1.0f;
	}
	else
	{	// Do the full lighthing and texture calculation

		// inout vec4 outColour, in vec4 surfaceColour, in vec4 vertWorldPosition, in vec3 vertNormal
		calcLightContrib( final_outColour, surfaceColour, ex_Position, ex_Normal );
		// "Normal" lighting, no transparency
		final_outColour.a = 1.0f;
	}// if ( ex_DebugObjectColourOverride.w <> 0.0f )

	
	// For debugging
	//  final_outColour = ex_Color; 
	//final_outColour = vec4( ex_Normal.xyz, 1.0f );


	//final_outColour *= 0.01f;	// Drop the colours to almost black
	//final_outColour.xy += ex_TexUV_x2.xy;

	//final_outColour.xyz += textColour_0.xyz;
	//final_outColour.xyz += surfaceColor.xyz;

	//calcLightContrib(final_outColour, surfaceColour, ex_Position, ex_Normal);
}
