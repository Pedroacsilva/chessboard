#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 colors;

uniform mat4 u_MVP;


void main(){
	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0f);
}


#shader fragment
#version 330 core

out vec4 f_Color;
void main(){
	f_Color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
}
