// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;

uniform sampler2D u_texture;

void main()
{

	vec4 texture_color = texture2D(u_texture, v_uv);

	gl_FragColor = texture_color;
}
