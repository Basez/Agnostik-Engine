#version 330 core
 
layout(location=0) in vec3 in_position;
layout(location=2) in vec3 in_normal;
layout(location=8) in vec2 in_texcoord;
 
out vec4 v2f_normalW;
out vec2 v2f_texcoord;
 
// Model, View, Projection matrix.
uniform mat4 uModelViewProjectionMatrix;
uniform mat4 uModelMatrix;

void main()
{
	gl_Position = uModelViewProjectionMatrix * vec4(in_position, 1);
 
	v2f_normalW = uModelMatrix * vec4(in_normal, 0); 
	v2f_texcoord = in_texcoord;
}
