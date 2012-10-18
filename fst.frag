#version 430 core
#define FRAG_COLOR	0
layout(location = FRAG_COLOR, index = 0) out vec4 Color;
in float vb;
void main() 
{
  Color = vec4(vb,1,vb,1);
}
