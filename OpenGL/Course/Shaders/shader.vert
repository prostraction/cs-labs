#version 330

layout (location = 0) in vec3 pos;		// position of each vertex
layout (location = 1) in vec2 tex;		// texels for textures
layout (location = 2) in vec3 normal;	// normals for diffuse lighting

out vec4 vColor;						// color for fragment shaders
out vec2 TexCoord;						// texels U V co-ordinates
out vec3 Normal;						// normals will be interpolated for Phong Shading in fragment shader. "flat out..." for Flat Shading
out vec3 FragPos;						// fragment position in the world space
out vec4 DirectionalLightSpacePos;		// where is the fragment position is related to the light

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;	// (projection * view) of a camera in shadow map

void main()
{
	gl_Position =  projection * view * model * vec4(pos, 1.0);
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);

	vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);				// clamp sets color for pixel based to it's x,y,z position (RGB 0.0f-1.0f)
	
	TexCoord = tex;
	Normal = mat3(transpose(inverse(model))) * normal;			// normals are directions related to model's position (last column of model). Transpose for countering non-uniform scales

	FragPos = (model * vec4(pos, 1.0)).xyz;						// we don't need projection and view matrices, we just need the model's position
}