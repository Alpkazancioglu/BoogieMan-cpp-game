#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture0;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(gl_FragCoord.z),1.0f);
    vec4 textureColor = texture(texture0 , fragTexCoord);
    if(textureColor.w < 0.5)
    {
        discard;
    }
  
}