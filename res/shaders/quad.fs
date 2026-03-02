varying vec2 v_uv;
uniform int u_task;
uniform int u_subtask;
uniform sampler2D u_texture;
uniform float u_time;


void main()
{

vec3 color = vec3(1.0, 0.0, 0.0);
vec4 texture_color;
float pi = 3.14159265359;

if (u_task == 1){
    // Drawing formulas
    if (u_subtask == 1){
    // 1 a
        color = vec3(v_uv.x, 0.0, 1.0-v_uv.x);
    } else if (u_subtask == 2){
    // 1 b
        vec2 center = vec2(0.5,0.5);
        color = vec3(distance(v_uv, center));
    } else if (u_subtask == 3){
    // 1 c
        vec2 st = gl_FragCoord.xy / vec2(640, 480);
        float r = sin(8.0 * 3.14159265359 * st.x);
        float g = 0.0;
        float b = sin(8.0 * 3.14159265359 * st.y);
        color = vec3(r, g, b);
    } else if (u_subtask == 4){
    // 1 d
        float x = floor(v_uv.x*20.0);
        float y = floor(v_uv.y*20.0);
        color = vec3((x/20.0) ,(y/20.0) , 0.0);
    } else if (u_subtask == 5){
    // 1 e
        float sum = floor(v_uv.x * 16.0) + floor(v_uv.y * 16.0);
        color = vec3(mod(sum,2.0), mod(sum,2.0), mod(sum,2.0));
    } else if (u_subtask == 6){
    // 1 f
        float wave = 0.5 + 0.25 *sin(v_uv.x * 2.0 * pi);
        float step_up = step(wave, v_uv.y);
        float step_down = 1.0 - step_up;
        float gradient = (1.0 - v_uv.y) * step_up + v_uv.y * step_down;
        color = vec3(0.0, gradient, 0.0);
    }
    gl_FragColor = vec4(color, 1.0);
} else if (u_task == 2){
    // Image filters
    texture_color = texture2D(u_texture, v_uv);
    if (u_subtask == 1){
    // 2 a
        float med = (texture_color.x+texture_color.y+texture_color.z) / 3.0;
        texture_color = vec4(med,med,med,1.0);
    } else if (u_subtask == 2) {
    // 2 b
        texture_color = vec4(1.0-texture_color.x, 1.0-texture_color.y, 1.0-texture_color.z, 1.0);
    } else if (u_subtask == 3) {
    // 2 c
        float med2 = (texture_color.x+texture_color.y) / 2.0;
        texture_color = vec4(med2, med2, 0.0, 1.0);
    } else if (u_subtask == 4) {
    // 2 d
        float sum = texture_color.x+texture_color.y+texture_color.z;
        float sum2 = sum / 1.5;
        texture_color = vec4(floor(sum2), floor(sum2), floor(sum2), 1.0);
    } else if (u_subtask == 5) {
    // 2 e
        float d = length(v_uv - 0.5);
        vec4 vignette = vec4(0.0, 0.0, 0.0, 1.0);
        texture_color = mix(texture_color, vignette, d);
    } else if (u_subtask == 6){
    // 2 f
        float offset = 0.005;
	    texture_color = (texture2D(u_texture, v_uv + offset*vec2(-1.0, -1.0))
        + texture2D(u_texture, v_uv + offset*vec2(0.0, -1.0))
        + texture2D(u_texture, v_uv + offset*vec2(1.0, -1.0))
        + texture2D(u_texture, v_uv + offset*vec2(-1.0, 0.0))
        + texture2D(u_texture, v_uv + offset*vec2(0.0,  0.0))
        + texture2D(u_texture, v_uv + offset*vec2(1.0,  0.0))
        + texture2D(u_texture, v_uv + offset*vec2(-1.0, 1.0))
        + texture2D(u_texture, v_uv + offset*vec2(0.0,  1.0))
        + texture2D(u_texture, v_uv + offset*vec2(1.0,  1.0))) / 9.0;
    }
    gl_FragColor = texture_color;
} else if (u_task == 3){
    // Image transformations
    texture_color = texture2D(u_texture, v_uv);
    if (u_subtask == 1){
    // Dynamic brightness animation
        float light = 0.5*cos(v_uv.x + u_time);
        texture_color = vec4(texture_color.x+light, texture_color.y+light, texture_color.z+light, texture_color.w);
    } else if (u_subtask == 2){
    // Chromatic Deformation animation
        float offset = 0.008*cos(v_uv.x * 20.0 + u_time * 4.0);
        float r = texture(u_texture, v_uv - vec2(offset, 0.0)).x;
        float g = texture(u_texture, v_uv).y;
        float b = texture(u_texture, v_uv + vec2(0.0, -offset)).z;
        texture_color = vec4(r,g,b,1.0);
    } else if (u_subtask == 3){
	texture_color = texture2D(u_texture, v_uv * abs(sin(u_time*0.8)));
	}
    gl_FragColor = texture_color;
}

}
