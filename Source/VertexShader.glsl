
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 coord;

out vec2 pos;
out vec2 UV;

//uniform mat4 MVP;

void main()
{
	//gl_Position = MVP*vec4(position, 1.0f);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = vec4(position.x, position.y, 0.0 ,1.0);
	pos = position;
	UV = coord;
}

