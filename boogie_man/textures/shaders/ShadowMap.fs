#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture0;
out vec4 FragColor;

void main()
{
    vec4 textureColor = texture(texture0 , fragTexCoord);
    if(textureColor.w < 0.5)
    {
        discard;
    }
}