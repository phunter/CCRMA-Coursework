
uniform samplerCube environMap;

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
