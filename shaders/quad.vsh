#version 330

layout (location = 0) in vec3 pos;
out vec3 f_pos;

void main(){
    gl_Position = vec4(pos.x*2, pos.y*2.0, pos.z, 1.0);
    f_pos = pos;
}
