#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float waterSin;

void main()
{
  vec3 Pos = aPos;
  Pos.y += waterSin / 4 -0.5;
	gl_Position = projection * view * model * vec4(Pos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
