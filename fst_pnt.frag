#version 430 core

layout(location = 5) in float nerta;


layout(location = 0) out vec4 Color;

void main() 
{
  Color = vec4(0.1,nerta,0.1,1);
}
