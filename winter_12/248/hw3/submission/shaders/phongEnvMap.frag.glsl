// This is a texture sampler.  It lets you sample textures!  The keyword
// "uniform" means constant - sort of.  The uniform variables are the same
// for all fragments in an object, but they can change in between objects.
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform samplerCube environMap;

// Diffuse, ambient, and specular materials.  These are also uniform.
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 Ka;
uniform float alpha;

uniform mat4 viewMatrix;

// These are values that OpenGL interpoates for us.  Note that some of these
// are repeated from the fragment shader.  That's because they're passed
// across.
varying vec3 normal;
varying vec3 eyePosition;

void main() {

	vec3 N = normalize(normal);
	vec3 V = normalize(-eyePosition);
    
    vec3 R = normalize(reflect(V,N));
    
    vec3 R_inv = (viewMatrix * vec4(R,0)).xyz;
    
    gl_FragColor = vec4(textureCube(environMap, R_inv).xyz, 1) + vec4(-.02, -.03, -1, 0);
}
