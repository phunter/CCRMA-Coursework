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

// These are values that OpenGL interpoates for us.  Note that some of these
// are repeated from the fragment shader.  That's because they're passed
// across.
varying vec2 texcoord;
varying vec3 normal;
varying vec3 tangent;
varying vec3 eyePosition;

void main() {

    vec3 Xn = textureCube(environMap, texcoord).rgb;
    
    // handle normal mapping stuff
    vec3 Tn = texture2D(normalMap, texcoord).rgb;
    Tn = 2.0 * (Tn-.5);
  
    normalize(normal);
    normalize(tangent);
    vec3 bitangent = cross(normal, tangent);
    normalize(bitangent);
    
    
    mat3 TBN;
    TBN[0] = tangent;
    TBN[1] = bitangent;
    TBN[2] = normal;
    vec3 TranNorm = TBN * Tn;

    vec3 N = normalize(TranNorm);
    
	vec3 L = normalize(gl_LightSource[0].position.xyz);
	vec3 V = normalize(-eyePosition);
		
	// Calculate the diffuse color coefficient, and sample the diffuse texture
	float Rd = max(0.0, dot(L, N));
	vec3 Td = texture2D(diffuseMap, texcoord).rgb;
	vec3 diffuse = Rd * Kd * Td * gl_LightSource[0].diffuse.rgb;
	
	// Calculate the specular coefficient
	vec3 R = reflect(-L, N);
	float Rs = pow(max(0.0, dot(V, R)), alpha);
	vec3 Ts = texture2D(specularMap, texcoord).rgb;
	vec3 specular = Rs * Ks * Ts * gl_LightSource[0].specular.rgb;
		
	// Ambient is easy
	vec3 ambient = Ka * gl_LightSource[0].ambient.rgb;

	// This actually writes to the frame buffer
    gl_FragColor = vec4(diffuse + specular + ambient, 1);
//    gl_FragColor = vec4(Xn, 1);
//    gl_FragColor = vec4(textureCube(environMap, reflect(eyePosition, N)).rgb, 1);
}
