#version 330 core 

layout (location = 0) in vec3 Vpos;
layout (location = 1) in vec2 Texcoord;

out vec2 fragTexCoord;

uniform vec3 offsets[500];
uniform mat4 cameraMat;
uniform mat4 modelMat;

void main()
{
   fragTexCoord = Texcoord;
   vec2 offset;
   
   offset = offsets[gl_InstanceID].xy;
   
   vec3 pos =  Vpos;
   vec3 finalPos = vec3(offsets[gl_InstanceID].z) * vec3(pos.x + offset.x, pos.y + offset.y , pos.z);
   gl_Position = cameraMat * modelMat * vec4(finalPos.x, finalPos.y , finalPos.z , 1.0) ;
}