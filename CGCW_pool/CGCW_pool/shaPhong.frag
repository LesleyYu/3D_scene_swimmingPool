#version 450 core

layout (location = 0) out vec4 fColour;

in vec3 col;
in vec3 nor;
in vec3 FragPosWorldSpace;
in vec4 FragPosProjectedLightSpace;

uniform sampler2D shadowMap;

uniform vec3 sunlightDirection;
uniform vec3 sunlightColor;
uniform vec3 sunlightPos;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

float shadowOnFragment(vec4 FragPosProjectedLightSpace) {
        vec3 ndc = FragPosProjectedLightSpace.xyz / FragPosProjectedLightSpace.w;
        vec3 ss = (ndc+1) * 0.5;
        float fragDepth = ss.z;
        float litDepth = texture(shadowMap, ss.xy).r;
        vec3 Nnor = normalize(nor);
        vec3 Ntolight = normalize(-lightDirection);
        float bias = max(0.05 * (1.0 - dot(Nnor, Ntolight)), 0.005);
        float shadow = 0.f;
        shadow = fragDepth > (litDepth + bias) ? 1.0 : 0.0;
        if (fragDepth > 1)
                shadow = 0.f;
        return shadow;
}

float CalculateDirectionalIllumination() {
	float ambient = 0.2f;

	vec3 Nnor = normalize(nor);		// normalised surface normal

	vec3 Nto_light = -(normalize(sunlightDirection));	// negated normalized light direction
	float diffuse = max(dot(Nnor, Nto_light), 0.f);

	vec3 Nfrom_light = normalize(sunlightDirection);	// normalized light direction
	vec3 NrefLight = reflect(Nfrom_light, Nnor);	// Reflect light on the surface
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);	// normalized camera direction
	float specular = pow(max(dot(NcamDirection, NrefLight), 0.f), 128.f);

	float shadow = shadowOnFragment(FragPosProjectedLightSpace);	// shadow
	// float phong = ambient + diffuse + specular;
	 float phong = ambient + (1.f - shadow)*(diffuse + specular);

	return phong;
}

float CalculatePositionalIllumination() {
	float ambient = 0.1f;

	vec3 Nnor = normalize(nor);		// normalised surface normal

	vec3 Nto_light = normalize(lightPos - FragPosWorldSpace);	// normalized light position
	float diffuse = max(dot(Nnor, Nto_light), 0.f);

	vec3 Nfrom_light = - Nto_light;	// normalized light direction
	vec3 NrefLight = reflect(Nfrom_light, Nnor);	// Reflect light on the surface
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);	// normalized camera direction
	float specular = pow(max(dot(NcamDirection, NrefLight), 0.f), 128.f);

	float d = length(lightPos - FragPosWorldSpace);	// distance between the fragment and the light source
	float attenuation = 1.f / (1.5f + 0.05f * d + 0.02f * d * d);	
	// here, value of constant attenuation(1.5), linear attenuation(0.05) and quadratic attenuation(0.02) are fixed.

	float phong = (ambient + diffuse + specular) * attenuation;
	return phong;
}

float CalculateSpotIllumination() {
	float ambient = 0.1f;

	vec3 Nnor = normalize(nor);		// normalised surface normal

	vec3 Nto_light = normalize(lightPos - FragPosWorldSpace);	// normalized light position
	float diffuse = max(dot(Nnor, Nto_light), 0.f);

	vec3 Nfrom_light = - Nto_light;	// normalized light direction
	vec3 NrefLight = reflect(Nfrom_light, Nnor);	// Reflect light on the surface
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);	// normalized camera direction
	float specular = pow(max(dot(NcamDirection, NrefLight), 0.f), 128.f);

	float d = length(lightPos - FragPosWorldSpace);	// distance between the fragment and the light source
	float attenuation = 1.f / (1.5f + 0.05f * d + 0.02f * d * d);

	float phi = cos(radians(15.f)); 	// calculate PHI using cosine and cut_off angle
	vec3 NSpotDir = normalize(lightDirection);	// normalized lightDirection
	float theta = dot(Nfrom_light, NSpotDir);

	float phong;
	if(theta > phi)
		phong = (ambient + diffuse + specular) * attenuation;
	else
		phong = ambient * attenuation;
	return phong;
}

void main()
{
	float phongSun = CalculateDirectionalIllumination();
	float phongSpot = CalculateSpotIllumination();

	fColour = vec4(col * (phongSun * sunlightColor + phongSpot * lightColor), 1.f);
}
