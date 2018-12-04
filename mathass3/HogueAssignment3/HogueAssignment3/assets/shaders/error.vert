#version 420

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

layout(location = 0) in vec3 in_vert;

out vec4 pos;

void main()
{

	pos = (uView * uModel * vec4(in_vert, 1.0f));

	gl_Position = uProj * pos;

}
