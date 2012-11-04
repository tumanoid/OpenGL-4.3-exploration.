#version 430 core

layout(location = 1) in vec3 vb;
layout(location = 0) out vec4 Color;

void main() 
{
  float r = 0.5+vb.x*0.5;  
  float g = 0.5+vb.y*0.5;  
  float b = 0.5+vb.z*0.5;  

  Color = vec4(r,g,b,0.1);
}
