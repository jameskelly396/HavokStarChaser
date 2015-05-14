// This is taken from the OpenGL Shading Language, 3rd Ed book
// Pages 273-279

void DirectionalLight( const in int i, 
                       const in vec3 normal,
					   inout vec4 ambient,
					   inout vec4 diffuse,
					   inout vec4 specular )
{
	float nDotVP;	// normal . light direction
	float nDotHV;	// normal . light half vector
	float pf;		// power factor
	
	nDotVP = max(0.0f, 
	             dot(normal, normalize(vec3(LightSource[i].position))));
				 
	nDotHV = max(0.0f, dot(normal, vec3(LightSource[i].halfVector)));
	
	if (nDotVP == 0.0f)
	{
		pf = 0.0f;
	}
	else
	{
		pf = pow(nDotHV, FrontMaterial.shininess);
	}
	
	ambient += LightSource[i].ambient;
	diffuse += LightSource[i].diffuse * nDotVP;
	specular += LightSource[i].specular * pf;
}