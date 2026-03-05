// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal;
varying vec3 v_ip;

uniform sampler2D u_texture;

void main()
{
	// Set the ouput color per pixel
	vec3 color = v_ip;

	gl_FragColor = vec4( color, 1.0 );
}
