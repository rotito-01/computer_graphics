varying vec2 v_uv;
uniform int u_task;
uniform int u_subtask;
uniform sampler2D u_texture;
uniform vec2 u_res;

void main()
{
	vec2 uv = v_uv.xy/u_res.xy;
	vec3 color = vec3(1.0, 0.0, 0.0);
	vec4 texture_color;
	float pi = 3.14159265359;

	if (u_task == 1){
// Drawing formulas
		if (u_subtask == 1){
		// 1 a
			color = vec3(uv.x, 0.0, 1-uv.x);
		} else if (u_subtask == 2){
		// 1 b
			vec2 center = vec2(0.5,0.5);
			color = vec3(distance(uv, center));
		} else if (u_subtask == 3){
		// 1 c
            float r = sin(8.0 * pi * uv.x);
			float g = 0;
            float b = sin(8.0 * pi * uv.y);
            color = vec3(r, g, b);
		} else if (u_subtask == 4){
		// 1 d
			float x = floor(uv.x*20);
			float y = floor(uv.y*20);
			color = vec3((x/20) ,(y/20) , 0.0);
		} else if (u_subtask == 5){
		// 1 e
			float sum = floor(uv.x * 16) + floor(uv.y * 16);
			color = vec3(mod(sum,2.0), mod(sum,2.0), mod(sum,2.0));
		} else if (u_subtask == 6){
		// 1 f
			float wave = 0.5 + 0.25 *sin(uv.x * 2.0 * pi);
			float step_up = step(wave, uv.y);
			float step_down = 1.0 - step_up;
			float gradient = (1 - uv.y) * step_up + uv.y * step_down;
			color = vec3(0.0, gradient, 0.0);
		} else if (u_subtask == 7){
		// EXTRA
			uv.x *= u_res.x/u_res.y;
			vec3 color = vec3(0.0);
			float d = 0.0;

			// Remap the space to -1. to 1.
			uv = uv *2.-1.;

			// Make the distance field
			d = length( abs(uv)-.3 );

			color = vec3(fract(d*10.0));
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
