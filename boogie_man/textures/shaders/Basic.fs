#version 330 core

in vec2 texcoord;
uniform sampler2D tex;

out vec4 FragColor;

uniform vec4 tint;

void main()
{ 
  FragColor = tint * texture(tex,texcoord);
}