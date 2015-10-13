// skybox.vert
#version 420 core
 
layout(location=0) in vec3 in_position;				// mesh specific
layout(location=1) in vec3 in_normal;				// mesh specific
layout(location=2) in vec3 in_tangent;				// mesh specific
layout(location=3) in vec3 in_bitangent;			// mesh specific
layout(location=4) in vec2 in_texcoord;				// mesh specific
 
out vec2 v2f_texcoord;
 
// Model, View, Projection matrix.
uniform mat4 uModelViewProjectionMatrix;			// mesh specific

void main()
{
	gl_Position = uModelViewProjectionMatrix * vec4(in_position, 1);
 
	v2f_texcoord = in_texcoord;
}
