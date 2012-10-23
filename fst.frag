#version 430 core

layout(location = 1) in float vb;

layout(location = 0) out vec4 Color;

void main() 
{
  Color = vec4(vb,1,vb,1);
}
