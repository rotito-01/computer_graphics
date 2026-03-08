// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;

// Gouraud shading parameters
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform vec3 u_kd;
uniform vec3 u_eye;
uniform vec3 u_ambient;
uniform vec3 u_light_pos;
uniform vec3 u_light_col;
uniform float u_shine;

// Variables to pass to the fragment shader
varying vec3 v_ip;

//here create uniforms for all the data we need here

void main()
{	
	// Convert local position to world space
	vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;
	
	// Convert local normal to world space
	vec3 world_normal = (u_model * vec4( gl_Normal.xyz, 0.0)).xyz;

	vec3 w_normal = normalize(world_normal);

	vec3 l = (u_light_pos - world_position);
	vec3 l_normal = normalize(l);

	vec3 v = (u_eye - world_position);
	vec3 v_normal = normalize(v);

	vec3 r = reflect(-l_normal,w_normal);

	float d = distance(world_position, u_light_pos);

	vec3 amb = u_ka * u_ambient;

	vec3 dif = clamp(dot(l_normal, w_normal),0.0 , 1.0)*u_kd;

	vec3 spec =  u_ks * pow(clamp(dot(r, v_normal), 0.0, 1.0), u_shine);

	vec3 ip = amb + (u_light_col/(d*d))*(dif + spec);

	v_ip = ip;

	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
}