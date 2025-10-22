#version 330
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 pos;				// position of each vertex

uniform mat4 model;
uniform mat4 directionalLightTransform;			// (projection * view) of a camera

void main()
{
	gl_Position = directionalLightTransform * model * vec4(pos, 1.0);
}