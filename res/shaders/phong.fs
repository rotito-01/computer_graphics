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

uniform mat4 u_model;
uniform sampler2D u_normal_texture;
uniform float u_use_texture;
uniform float u_use_specular;
uniform float u_use_normal;

void main()
{
	// Set the ouput color per pixel
	vec3 color = vec3(1,0,0);

    vec3 w_normal;
    
    if (u_use_normal == 1.0) {
        vec3 n_tex = texture2D(u_normal_texture, v_uv).rgb;
        n_tex = n_tex * 2.0 - 1.0; // convert normal from texture!! [-1,1]
        w_normal = normalize((u_model * vec4(n_tex, 0.0)).xyz);
    }
    else {
        w_normal = normalize(v_world_normal);
    }
    
    vec3 ka = u_ka;
    vec3 kd = u_kd;
    vec3 ks = u_ks;
    
    if (u_use_texture == 1.0) { // as the pdf says, replace ka and kd with the texture color
        vec3 tex_color = texture2D(u_texture, v_uv).rgb;
        ka = tex_color;
        kd = tex_color;
    }
    // we replace ks by the alpha of the color texture
    if (u_use_specular == 1.0) {
        float spec_alpha = texture2D(u_texture, v_uv).a;
        ks = vec3(spec_alpha);
    }
    
	vec3 l = (u_light_pos - v_world_position);
	vec3 l_normal = normalize(l);

	vec3 v = (u_eye - v_world_position);
	vec3 v_normal = normalize(v);

	vec3 r = reflect(-l_normal,w_normal);

	float d = distance(v_world_position, u_light_pos);

	vec3 amb = ka * u_ambient;

	vec3 dif = clamp(dot(l_normal, w_normal),0.0 , 1.0)*kd;

	vec3 spec =  ks * pow(clamp(dot(r, v_normal), 0.0, 1.0), u_shine);

	vec3 ip = amb + (u_light_col/(d*d))*(dif + spec);

	color = ip;

	gl_FragColor = vec4( color, 1.0 );
}
