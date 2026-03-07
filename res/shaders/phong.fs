// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

uniform sampler2D u_texture;

// Phong shading parameters
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform vec3 u_kd;
uniform vec3 u_eye;
uniform vec3 u_ambient;
uniform vec3 u_light_pos;
uniform vec3 u_light_col;
uniform float u_shine;

void main()
{
	// Set the ouput color per pixel
	vec3 color = vec3(1,0,0);

	vec3 w_normal = normalize(v_world_normal);

	vec3 l = (u_light_pos - v_world_position);
	vec3 l_normal = normalize(l);

	vec3 v = (u_eye - v_world_position);
	vec3 v_normal = normalize(v);

	vec3 r = reflect(-l_normal,w_normal);

	float d = distance(v_world_position, u_light_pos);

	vec3 amb = u_ka * u_ambient;

	vec3 dif = clamp(dot(l_normal, w_normal),0.0 , 1.0)*u_kd;

	vec3 spec =  u_ks * pow(clamp(dot(r, v_normal), 0.0, 1.0), u_shine);

	vec3 ip = amb + (u_light_col/(d*d))*(dif + spec);

	color = ip;

	gl_FragColor = vec4( color, 1.0 );
}
