varying vec2 v_uv;
uniform int u_task;
uniform int u_subtask;
uniform sampler2D u_texture;

void main()
{
	vec3 color = vec3(1.0, 0.0, 0.0);
	vec4 texture_color;

	if (u_task == 1){
// Drawing formulas
		if (u_subtask == 1){
		// 1 a
		color = vec3(v_uv.x, 0.0, 1-v_uv.x);
		} else if (u_subtask == 2){
		// 1 b
		vec2 center = vec2(0.5,0.5);
		color = vec3(distance(v_uv, center));
		} else if (u_subtask == 3){
		
		}
		gl_FragColor = vec4(color, 1.0);
	} else if (u_task == 2){
// Image filters
		if (u_subtask == 1){
		// 2 a
		texture_color = texture2D(u_texture, v_uv);
		}
		gl_FragColor = texture_color;
	}
	
	
}
