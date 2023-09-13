#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture0;
out vec4 FragColor;

void main()
{
   FragColor = vec4(1.0f,0.0f,0.0f,1.0f)* gl_FragCoord.x;
}