#version 330 core

in vec2 fragTexCoord;

in vec2 texcoord;
uniform sampler2D texture0;
uniform sampler2D skytexture;

out vec4 FragColor;
uniform vec4 tint;


void main()
{
    float AmbientAmount = 0.4f;

    vec2 uv = -fragTexCoord.xy / vec2(1080,1920);
    vec4 color = vec4(texture(skytexture, uv / gl_FragCoord.xy).xyz * 1.5f, 1.0f);

    vec3 colorEvened = vec3((color.x + color.y + color.z) / 3);
    colorEvened = colorEvened.x < AmbientAmount ? vec3(AmbientAmount) : colorEvened;
    color = mix(vec4( colorEvened, 1.0f) , color , 0.05) ;

    FragColor = tint * color * texture(texture0,fragTexCoord);
}
