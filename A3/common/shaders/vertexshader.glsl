#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 projection;

void main(){
	gl_PointSize = 1.0;
	gl_Position = projection*vec4(pos.x,pos.y,pos.z,1.0);
}