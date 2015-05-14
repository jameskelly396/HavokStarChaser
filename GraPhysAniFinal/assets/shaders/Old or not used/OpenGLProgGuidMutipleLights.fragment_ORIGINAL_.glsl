// This shader is from the OpenGL Programming Guide, 8th edition, page 378 - 379

// FRAGMENT shader for multiple lights

struct LightProperties
{
	bool isEnabled;
	bool isLocal;
	bool isSpot;
	vec3 ambient;
	vec3 color;
	vec3 position;
	vec3 halfVector;
	vec3 coneDirection;
	float spotCosCutoff;
	float spotExponent;
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

in vec4 Position;
in vec3 Normal;			// Note Normal is a vec3
in vec4 Color;

out vec4 FragColor;

void main()
{
	FragColor = Color;

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
		if ( Lights[light].isLocal )
		{
			lightDirection = lightDirection - vec3(Position);
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
			}
			
			halfVector = normalize( lightDirection + EyeDirection );
		}
		else
		{
			halfVector = Lights[light].halfVector;
		}
		
		float diffuse = max( 0.0, dot(Normal, lightDirection) );
		float specular = max( 0.0, dot(Normal, halfVector) );
		
		if (diffuse == 0.0)
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
	}
	
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0) );
	
	FragColor = vec4(rgb, Color.a);
}
