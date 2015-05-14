// This shader is from the OpenGL Programming Guide, 8th edition, page 378 - 379

// FRAGMENT shader for multiple lights

#version 430

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
	float range;		// Limit of light range (for performance)
};

// the set of lights to apply, per invocation of this shader
const int NUMLIGHTS = 2;
uniform LightProperties Lights[NUMLIGHTS];

uniform float Shininess;
uniform float Strength;
uniform vec3 EyeDirection;

// Added to blend the textures 
uniform float textureMix;
uniform bool hasTexture;

//Transparency flag
uniform bool isTransparent;
uniform float transparency;

// ADDED on October 17, 2014
uniform samplerCube skyMapTexture;

uniform sampler2D texture_00;
uniform sampler2D texture_01;
uniform sampler2D texture_02;
uniform sampler2D texture_03;
uniform sampler2D texture_04;
uniform sampler2D texture_05;
uniform sampler2D texture_06;
uniform sampler2D texture_07;
uniform sampler2D texture_08;
uniform sampler2D texture_09;
uniform sampler2D texture_10;
uniform sampler2D texture_11;
uniform sampler2D texture_12;
uniform sampler2D texture_13;
uniform sampler2D texture_14;
uniform sampler2D texture_15;
uniform sampler2D texture_16;
uniform sampler2D texture_17;
uniform sampler2D texture_18;
uniform sampler2D texture_19;
uniform sampler2D texture_20;
uniform sampler2D texture_21;

// For shadow mapping
uniform sampler2DShadow shadow_tex_00;
uniform sampler2DShadow shadow_tex_01;
uniform sampler2DShadow shadow_tex_02;
uniform sampler2DShadow shadow_tex_03;
uniform sampler2DShadow shadow_tex_04;
uniform sampler2DShadow shadow_tex_05;
uniform sampler2DShadow shadow_tex_06;
uniform sampler2DShadow shadow_tex_07;
uniform sampler2DShadow shadow_tex_08;
uniform sampler2DShadow shadow_tex_09;
uniform sampler2DShadow shadow_tex_10;
uniform sampler2DShadow shadow_tex_11;
uniform sampler2DShadow shadow_tex_12;
uniform sampler2DShadow shadow_tex_13;
uniform sampler2DShadow shadow_tex_14;
uniform sampler2DShadow shadow_tex_15;
uniform sampler2DShadow shadow_tex_16;
uniform sampler2DShadow shadow_tex_17;
uniform sampler2DShadow shadow_tex_18;
uniform sampler2DShadow shadow_tex_19;
uniform sampler2DShadow shadow_tex_20;
uniform sampler2DShadow shadow_tex_21;


uniform bool bIsSkyMap;
uniform bool bIsParticle;
uniform bool bUseDebugColour;
uniform vec4 debugObjectColour;
uniform bool bGenShadowFromLight;	// For shadow generation
uniform int textureID;
uniform int blend;

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
	bool bInRange = true;
	
	// loop over all the lights
	for ( int light = 0; light < NUMLIGHTS; ++light )
	{ 
		if ( ! Lights[light].isEnabled )
			continue;
			
			
		bInRange = true;
		
		vec3 halfVector;
		vec3 lightDirection = Lights[light].position;
		float attenuation = 1.0;
	
		// for local lights, compute per-fragment direction,
		// half-vector, and attenuation
		if ( Lights[light].isLocal )			// aks Directional lighting
		{

			lightDirection = lightDirection - vec3(ex_Position);
			float lightDistance = length(lightDirection);
			
			if ( lightDistance > Lights[light].range )
			{
				bInRange = false;
			}
			else
			{	// Is in range
			
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
			}// if ( lightDistance > Lights[light].range )
		}
		else
		{
			halfVector = Lights[light].halfVector;
		} // if ( Lights[light].isLocal )	

	
		if ( bInRange )
		{	
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
		}// if ( bInRange )
		
	} // for ( int light = 0, ...

	vec3 rgb = min(surfaceColour.rgb * scatteredLight + reflectedLight, vec3(1.0) );	//vec3 rgb = vec3(1.0f, 1.0f, 1.0f);

	outColour = vec4(rgb, surfaceColour.a);
	//outColour *= 0.1f;
	//outColour.xyz += Lights[0].color;
	return;
}

// FRAGMENT

void main(void)
{
	vec4 surfaceColour = ex_Color;
	
	vec4 textColour_00 = texture( texture_00, ex_TexUV_x2.xy );	 // sampler2D	
	vec4 textColour_01 = texture( texture_01, ex_TexUV_x2.xy );
	vec4 textColour_02 = texture( texture_02, ex_TexUV_x2.xy );
	vec4 textColour_03 = texture( texture_03, ex_TexUV_x2.xy );
	vec4 textColour_04 = texture( texture_04, ex_TexUV_x2.xy );	
	vec4 textColour_05 = texture( texture_05, ex_TexUV_x2.xy );
	vec4 textColour_06 = texture( texture_06, ex_TexUV_x2.xy );
	vec4 textColour_07 = texture( texture_07, ex_TexUV_x2.xy );
	vec4 textColour_08 = texture( texture_08, ex_TexUV_x2.xy );
	vec4 textColour_09 = texture( texture_09, ex_TexUV_x2.xy );
	vec4 textColour_10 = texture( texture_10, ex_TexUV_x2.xy );
	vec4 textColour_11 = texture( texture_11, ex_TexUV_x2.xy );
	vec4 textColour_12 = texture( texture_12, ex_TexUV_x2.xy );
	vec4 textColour_13 = texture( texture_13, ex_TexUV_x2.xy );
	vec4 textColour_14 = texture( texture_14, ex_TexUV_x2.xy );
	vec4 textColour_15 = texture( texture_15, ex_TexUV_x2.xy );
	vec4 textColour_16 = texture( texture_16, ex_TexUV_x2.xy );
	vec4 textColour_17 = texture( texture_17, ex_TexUV_x2.xy );
	vec4 textColour_18 = texture( texture_18, ex_TexUV_x2.xy );
	vec4 textColour_19 = texture( texture_19, ex_TexUV_x2.xy );
	vec4 textColour_20 = texture( texture_20, ex_TexUV_x2.xy );
	vec4 textColour_21 = texture( texture_21, ex_TexUV_x2.xy );

	//surfaceColour = textColour_0 + textColour_1;		// Replace colour with texture "colour"
	//surfaceColour = textColour_0 * textColour_1;		// Replace colour with texture "colour"
	//surfaceColour = mix(textColour_0, textColour_1, textureMix );

	if(blend == 0)
	{
		if(textureID == 0)
			surfaceColour = textColour_00;
		else if(textureID == 1)
			surfaceColour = textColour_01;
		else if(textureID == 2)
			surfaceColour = textColour_02;
		else if(textureID == 3)
			surfaceColour = textColour_03;
		else if(textureID == 4)
			surfaceColour = textColour_04;
		else if(textureID == 5)
			surfaceColour = textColour_05;
		else if(textureID == 6)
			surfaceColour = textColour_06;
		else if(textureID == 7)
			surfaceColour = textColour_07;
		else if(textureID == 8)
			surfaceColour = textColour_08;
		else if(textureID == 9)
			surfaceColour = textColour_09;
		else if(textureID == 10)
			surfaceColour = textColour_10;
		else if(textureID == 11)
			surfaceColour = textColour_11;
		else if(textureID == 12)
			surfaceColour = textColour_12;
		else if(textureID == 13)
			surfaceColour = textColour_13;
		else if(textureID == 14)
			surfaceColour = textColour_14;
		else if(textureID == 15)
			surfaceColour = textColour_15;
		else if(textureID == 16)
			surfaceColour = textColour_16;
		else if(textureID == 17)
			surfaceColour = textColour_17;
		else if(textureID == 18)
			surfaceColour = textColour_18;
		else if(textureID == 19)
			surfaceColour = textColour_19;
		else if(textureID == 20)
			surfaceColour = textColour_20;
	}
	else if(blend == 1)
	{
		if(textureID == 0)
			surfaceColour = textColour_00 * textColour_01;
		else if(textureID == 1)
			surfaceColour = textColour_01 * textColour_02;
		else if(textureID == 2)
			surfaceColour = textColour_02 * textColour_03;
		else if(textureID == 3)
			surfaceColour = textColour_03 * textColour_04;
		else if(textureID == 4)
			surfaceColour = textColour_04 * textColour_05;
		else if(textureID == 5)
			surfaceColour = textColour_05 * textColour_06;
		else if(textureID == 6)
			surfaceColour = textColour_06 * textColour_07;
		else if(textureID == 7)
			surfaceColour = textColour_07 * textColour_08;
		else if(textureID == 8)
			surfaceColour = textColour_08 * textColour_09;
		else if(textureID == 9)
			surfaceColour = textColour_09 * textColour_10;
		else if(textureID == 10)
			surfaceColour = textColour_10 * textColour_11;
		else if(textureID == 11)
			surfaceColour = textColour_11 * textColour_12;
		else if(textureID == 12)
			surfaceColour = textColour_12 * textColour_13;
		else if(textureID == 13)
			surfaceColour = textColour_13 * textColour_14;
		else if(textureID == 14)
			surfaceColour = textColour_14 * textColour_15;
		else if(textureID == 15)
			surfaceColour = textColour_15 * textColour_16;
		else if(textureID == 16)
			surfaceColour = textColour_16 * textColour_17;
		else if(textureID == 17)
			surfaceColour = textColour_17 * textColour_18;
		else if(textureID == 18)
			surfaceColour = textColour_18 * textColour_19;
		else if(textureID == 19)
			surfaceColour = textColour_19 * textColour_20;
		else if(textureID == 20)
			surfaceColour = textColour_20 * textColour_21;
	}
	else if(blend == 2)
	{
		if(textureID == 0)
			surfaceColour = textColour_00 + textColour_01;
		else if(textureID == 1)
			surfaceColour = textColour_01 + textColour_02;
		else if(textureID == 2)
			surfaceColour = textColour_02 + textColour_03;
		else if(textureID == 3)
			surfaceColour = textColour_03 + textColour_04;
		else if(textureID == 4)
			surfaceColour = textColour_04 + textColour_05;
		else if(textureID == 5)
			surfaceColour = textColour_05 + textColour_06;
		else if(textureID == 6)
			surfaceColour = textColour_06 + textColour_07;
		else if(textureID == 7)
			surfaceColour = textColour_07 + textColour_08;
		else if(textureID == 8)
			surfaceColour = textColour_08 + textColour_09;
		else if(textureID == 9)
			surfaceColour = textColour_09 + textColour_10;
		else if(textureID == 10)
			surfaceColour = textColour_10 + textColour_11;
		else if(textureID == 11)
			surfaceColour = textColour_11 + textColour_12;
		else if(textureID == 12)
			surfaceColour = textColour_12 + textColour_13;
		else if(textureID == 13)
			surfaceColour = textColour_13 + textColour_14;
		else if(textureID == 14)
			surfaceColour = textColour_14 + textColour_15;
		else if(textureID == 15)
			surfaceColour = textColour_15 + textColour_16;
		else if(textureID == 16)
			surfaceColour = textColour_16 + textColour_17;
		else if(textureID == 17)
			surfaceColour = textColour_17 + textColour_18;
		else if(textureID == 18)
			surfaceColour = textColour_18 + textColour_19;
		else if(textureID == 19)
			surfaceColour = textColour_19 + textColour_20;
		else if(textureID == 20)
			surfaceColour = textColour_20 + textColour_21;
	}
	else if(blend == 3)
	{
		if(textureID == 0)
			surfaceColour = textColour_00 - textColour_01;
		else if(textureID == 1)
			surfaceColour = textColour_01 - textColour_02;
		else if(textureID == 2)
			surfaceColour = textColour_02 - textColour_03;
		else if(textureID == 3)
			surfaceColour = textColour_03 - textColour_04;
		else if(textureID == 4)
			surfaceColour = textColour_04 - textColour_05;
		else if(textureID == 5)
			surfaceColour = textColour_05 - textColour_06;
		else if(textureID == 6)
			surfaceColour = textColour_06 - textColour_07;
		else if(textureID == 7)
			surfaceColour = textColour_07 - textColour_08;
		else if(textureID == 8)
			surfaceColour = textColour_08 - textColour_09;
		else if(textureID == 9)
			surfaceColour = textColour_09 - textColour_10;
		else if(textureID == 10)
			surfaceColour = textColour_10 - textColour_11;
		else if(textureID == 11)
			surfaceColour = textColour_11 - textColour_12;
		else if(textureID == 12)
			surfaceColour = textColour_12 - textColour_13;
		else if(textureID == 13)
			surfaceColour = textColour_13 - textColour_14;
		else if(textureID == 14)
			surfaceColour = textColour_14 - textColour_15;
		else if(textureID == 15)
			surfaceColour = textColour_15 - textColour_16;
		else if(textureID == 16)
			surfaceColour = textColour_16 - textColour_17;
		else if(textureID == 17)
			surfaceColour = textColour_17 - textColour_18;
		else if(textureID == 18)
			surfaceColour = textColour_18 - textColour_19;
		else if(textureID == 19)
			surfaceColour = textColour_19 - textColour_20;
		else if(textureID == 20)
			surfaceColour = textColour_20 - textColour_21;

		surfaceColour.a = 1.0f;
	}
	else if(blend == 4)
	{
		if(textureID == 0)
		{
			surfaceColour = textColour_00;
			surfaceColour.a = ( (textColour_01.r + textColour_01.g + textColour_01.b) / 3.0f );
		}
		else if(textureID == 1)
		{
			surfaceColour = textColour_01;
			surfaceColour.a = ( (textColour_02.r + textColour_02.g + textColour_02.b) / 3.0f );
		}
		else if(textureID == 2)
		{
			surfaceColour = textColour_02;
			surfaceColour.a = ( (textColour_03.r + textColour_03.g + textColour_03.b) / 3.0f );
		}
		else if(textureID == 3)
		{
			surfaceColour = textColour_03;
			surfaceColour.a = ( (textColour_04.r + textColour_04.g + textColour_04.b) / 3.0f );
		}
		else if(textureID == 4)
		{
			surfaceColour = textColour_04;
			surfaceColour.a = ( (textColour_05.r + textColour_05.g + textColour_05.b) / 3.0f );
		}
		else if(textureID == 5)
		{
			surfaceColour = textColour_05;
			surfaceColour.a = ( (textColour_06.r + textColour_06.g + textColour_06.b) / 3.0f );
		}
		else if(textureID == 6)
		{
			surfaceColour = textColour_06;
			surfaceColour.a = ( (textColour_07.r + textColour_07.g + textColour_07.b) / 3.0f );
		}
		else if(textureID == 7)
		{
			surfaceColour = textColour_07;
			surfaceColour.a = ( (textColour_08.r + textColour_08.g + textColour_08.b) / 3.0f );
		}
		else if(textureID == 8)
		{
			surfaceColour = textColour_08;
			surfaceColour.a = ( (textColour_09.r + textColour_09.g + textColour_09.b) / 3.0f );
		}
		else if(textureID == 9)
		{
			surfaceColour = textColour_09;
			surfaceColour.a = ( (textColour_10.r + textColour_10.g + textColour_10.b) / 3.0f );
		}
		else if(textureID == 10)
		{
			surfaceColour = textColour_10;
			surfaceColour.a = ( (textColour_11.r + textColour_11.g + textColour_11.b) / 3.0f );
		}
		else if(textureID == 11)
		{
			surfaceColour = textColour_11;
			surfaceColour.a = ( (textColour_12.r + textColour_12.g + textColour_12.b) / 3.0f );
		}
		else if(textureID == 12)
		{
			surfaceColour = textColour_12;
			surfaceColour.a = ( (textColour_13.r + textColour_13.g + textColour_13.b) / 3.0f );
		}
		else if(textureID == 13)
		{
			surfaceColour = textColour_13;
			surfaceColour.a = ( (textColour_14.r + textColour_14.g + textColour_14.b) / 3.0f );
		}
		else if(textureID == 14)
		{
			surfaceColour = textColour_14;
			surfaceColour.a = ( (textColour_15.r + textColour_15.g + textColour_15.b) / 3.0f );
		}
		else if(textureID == 15)
		{
			surfaceColour = textColour_15;
			surfaceColour.a = ( (textColour_16.r + textColour_16.g + textColour_16.b) / 3.0f );
		}
		else if(textureID == 16)
		{
			surfaceColour = textColour_16;
			surfaceColour.a = ( (textColour_17.r + textColour_17.g + textColour_17.b) / 3.0f );
		}
		else if(textureID == 17)
		{
			surfaceColour = textColour_17;
			surfaceColour.a = ( (textColour_18.r + textColour_18.g + textColour_18.b) / 3.0f );
		}
		else if(textureID == 18)
		{
			surfaceColour = textColour_18;
			surfaceColour.a = ( (textColour_19.r + textColour_19.g + textColour_19.b) / 3.0f );
		}
		else if(textureID == 19)
		{
			surfaceColour = textColour_19;
			surfaceColour.a = ( (textColour_20.r + textColour_20.g + textColour_20.b) / 3.0f );
		}
		else if(textureID == 20)
		{
			surfaceColour = textColour_20;
			surfaceColour.a = ( (textColour_21.r + textColour_21.g + textColour_21.b) / 3.0f );
		}
	}

	if(hasTexture == false)
		surfaceColour = vec4(1.0f);

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
		//surfaceColour = textColour_02;

		// Supah cool transparency to come...
		//final_outColour.a = 5.0f;	
		

		if(surfaceColour.r < 0.1f && surfaceColour.g < 0.1f && surfaceColour.b < 0.1f)
			discard;
		final_outColour = surfaceColour;
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
	else if ( isTransparent)
	{
		calcLightContrib( final_outColour, surfaceColour, ex_Position, ex_Normal );
		final_outColour.a = transparency;
	}
	else
	{	// Do the full lighthing and texture calculation

		// inout vec4 outColour, in vec4 surfaceColour, in vec4 vertWorldPosition, in vec3 vertNormal
		calcLightContrib( final_outColour, surfaceColour, ex_Position, ex_Normal );
		// "Normal" lighting, no transparency
		//final_outColour.a = 1.2f;
		final_outColour.a = surfaceColour.a;
		
		//final_outColour *= 0.01f;	// Drop the colours to almost
		//final_outColour.xyz += textColour_00.xyz;
		//final_outColour.a = 1.0f;
	}// if ( ex_DebugObjectColourOverride.w <> 0.0f )

	//if ( isTransparent)
	//	final_outColour.a = transparency;
	//else
	//	final_outColour.a = 1.0f;
	
	// For debugging
	//  final_outColour = ex_Color; 
	//final_outColour = vec4( ex_Normal.xyz, 1.0f );


	//final_outColour *= 0.01f;	// Drop the colours to almost black
	//final_outColour.xy += ex_TexUV_x2.xy;

	//final_outColour.xyz += textColour_00.xyz;
	//final_outColour.xyz += surfaceColor.xyz;

	//calcLightContrib(final_outColour, surfaceColour, ex_Position, ex_Normal);
}
