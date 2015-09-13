#version 330 core
 
in vec4 v2f_normalW; 	// Surface normal in world space.
in vec2 v2f_texcoord;
 
uniform vec4 uLightDirection; // Light's direction in world space.
uniform vec4 uLightColor; // Light's diffuse and specular contribution.
uniform vec4 uLightAmbient; // Global ambient contribution.
 
uniform vec4 uMaterialEmissive;
uniform vec4 uMaterialDiffuse;

uniform sampler2D textureSampler0;	// Diffuse

layout (location=0) out vec4 out_color;

void main()
{	
	// Compute the diffuse term.
	vec4 N = normalize( v2f_normalW );
	float NdotL = max( dot( N, uLightDirection ), 0 );
	vec4 Diffuse =  NdotL * uLightColor * uMaterialDiffuse;
	 
	vec4 lighting = Diffuse + uMaterialEmissive + uLightAmbient;
	vec4 finalColor = lighting * texture( textureSampler0, v2f_texcoord );
	
	out_color = finalColor;
}