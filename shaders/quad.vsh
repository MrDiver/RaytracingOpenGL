#version 330

in vec3 pos;
in vec2 uv;
out vec3 f_pos;
out vec2 f_uv;

void main(){
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    f_pos = pos;
    f_uv = uv;
}
