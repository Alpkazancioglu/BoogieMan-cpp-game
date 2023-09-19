#version 330 core

in vec2 fragTexCoord;
in vec3 currentpos;
in vec3 Normal;
in vec4 LightPosProj;

uniform sampler2D texture0;
uniform sampler2D skytexture;
uniform sampler2D shadowMap;
uniform vec3 LightPosition;
uniform vec4 tint;

out vec4 FragColor;

 float ShadowCalculation(vec4 fragPosLightSpace , vec3 lightDir ,vec3 normal)
 {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    float bias = max(0.02 * (1.0 - dot(normal, lightDir)), 0.003);  
    //float bias = 0.05;  
    
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
      for(int y = -1; y <= 1; ++y)
      {
         float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
         shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
      }    
    }
    shadow /= 9.0;


    return shadow;
 }

void main()
{
    float AmbientAmount = 0.4f;

    vec2 uv = -fragTexCoord.xy / vec2(1080,1920);
    vec4 color = vec4(texture(skytexture, uv / gl_FragCoord.xy).xyz * 1.5f, 1.0f);

    vec3 colorEvened = vec3((color.x + color.y + color.z) / 3);
    colorEvened = colorEvened.x < AmbientAmount ? vec3(AmbientAmount) : colorEvened;
    color = mix(vec4( colorEvened, 1.0f) , color , 0.05) ;
    
  
    vec3 lightvec = (LightPosition - currentpos);
    vec3 lightDirection = normalize(lightvec);

    float Shadow = ShadowCalculation(LightPosProj , lightDirection , Normal);

    vec3 projCoords = LightPosProj.xyz / LightPosProj.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float ambient = 1.5f;
    
    //FragColor = tint * texture(shadowMap , -fragTexCoord);
   // FragColor = vec4(vec3(texture(shadowMap , projCoords.xy).r),1.0f);
    //FragColor = vec4(vec3(gl_FragCoord.z),1.0f);
    //vec4 textureColor = texture(texture0 , fragTexCoord);
   // if(textureColor.w < 0.5)
   // {
    //  discard;
    //}
    //FragColor = vec4(vec3((1.0f - Shadow)),1.0f);
     //FragColor = tint * color * texture(texture0,fragTexCoord);

    FragColor = tint * color * vec4(vec3(1.0f - Shadow) + ambient,1.0f) * texture(texture0,fragTexCoord);

}
