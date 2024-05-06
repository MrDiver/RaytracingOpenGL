#version 330

in vec3 f_pos;
in vec2 f_uv;
out vec4 FragColor;

void main(){
    FragColor = vec4(f_uv, 0.0, 1.0);
}
