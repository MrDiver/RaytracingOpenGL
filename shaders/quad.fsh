#version 330

in vec3 f_pos;
out vec4 FragColor;

void main(){
    FragColor = vec4(f_pos, 1.0);
}
