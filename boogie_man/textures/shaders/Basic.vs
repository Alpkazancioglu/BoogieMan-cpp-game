#version 330 core 

layout (location = 0) in vec3 Vpos;
layout (location = 1) in vec2 Texcoord;
layout (location = 2) in vec3 aNormal;

out vec2 fragTexCoord;
out vec4 LightPosProj;
out vec3 currentpos;
out vec3 Normal;

uniform vec3 offsets[500];
uniform mat4 cameraMat;
uniform mat4 ratioMat;
uniform mat4 modelMat;

uniform mat4 LightProjection;

void main()
{
   fragTexCoord = Texcoord;

   mat3 normalMatrix = transpose(inverse(mat3(modelMat)));
   Normal = normalize(normalMatrix * aNormal);


   vec2 offset;
   offset = offsets[gl_InstanceID].xy;
   
   vec3 pos =  Vpos;
   vec3 finalPos = vec3(offsets[gl_InstanceID].z) * vec3(pos.x + offset.x, pos.y + offset.y , pos.z);

   currentpos = vec3(ratioMat * modelMat *  vec4(finalPos, 1.0f));
   LightPosProj = LightProjection * vec4(currentpos , 1.0f);

   gl_Position = cameraMat *  ratioMat * modelMat *  vec4(finalPos.x, finalPos.y , finalPos.z , 1.0) ;
   //gl_Position = LightPosProj;
}