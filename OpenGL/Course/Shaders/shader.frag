#version 330

in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int					pointLightCount;
uniform int					spotLightCount;

uniform DirectionalLight	directionalLight;
uniform PointLight			pointLights[MAX_POINT_LIGHTS];
uniform SpotLight			spotLights [MAX_SPOT_LIGHTS];

uniform sampler2D			theTexture;
uniform sampler2D			directionalShadowMap;
uniform Material			material;

uniform vec3				eyePosition;

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	// coordinates between -1 and 1
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	// coordinates between 0 and 1 (normalize)
	projCoords = (projCoords * 0.5) + 0.5; 

	// closest position to the light position
			//float closest = texture(directionalShadowMap, projCoords.xy).r; // .r -- first value
	// how far away camera is from the light
	float current = projCoords.z;

	// creating bias to overcome shadow acne
	vec3 normal		= normalize(Normal);
	vec3 lightDir	= normalize(light.direction);
	float bias		= max(0.05 * (1 - dot(normal, lightDir)), 0.005);

	// PCF algorithm for smoothing pixelated shadow textures
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	// finding the average of 3x3 block of pixels
	for (int x = -2; x <= 2; ++x)
	{
		for (int y = -2; y <= 2; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 25.0;

	//float shadow = current - bias > closest ? 1.0 : 0.0;
	// the point it is beyond the far plane
	if (projCoords.z > 1.0)
	{
		shadow = 0.0;
	}

	return shadow;

}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	// diffuse:
	// normalize to: A.B = |A| * |B| * cos(angle) --> 1 * 1 * cos(angle) --> cos(angle)
	// if Factor < 0.0f, Factor = 0.0f
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f); // searching for angle of light

	// or:
	// float diffuseFactor = max(dot(normalize(Normal), -normalize(direction)), 0.0f);

	//vec4 diffuseColor	= vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
	vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

	// specular:
	vec4 specularColor = vec4(0, 0, 0, 0);
	if (diffuseFactor > 0.0f)
	{
		vec3 fragToEye		 = normalize(eyePosition - FragPos);	// where we are relationed to the object
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f)
		{
			specularFactor	= pow(specularFactor, material.shininess);
			specularColor	= vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);	
}

vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 color = CalcLightByDirection(pLight.base, direction, 0.0f);
	// ax^2 + bx + c
	float attenuation = pLight.exponent * distance * distance + 
						pLight.linear * distance + 
						pLight.constant;
	return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);

	if (slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base);

		return color * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
		// 0 - 1
		// 0 - 2
		// 0.3 -> 0.6
	}
	else
	{
		return vec4(0,0,0,0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0,0,0,0);
	for (int i = 0; i < pointLightCount; i++)
	{
		totalColor += CalcPointLight(pointLights[i]);
	}
	return totalColor;
}

vec4 CalcSpotLights()
{
	vec4 totalColor = vec4(0,0,0,0);
	for (int i = 0; i < spotLightCount; i++)
	{
		totalColor += CalcSpotLight(spotLights[i]);
	}
	return totalColor;
}

void main()
{
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	finalColor += CalcSpotLights();
	color = texture(theTexture, TexCoord) * finalColor;
}