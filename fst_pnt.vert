#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in float Bright;

layout(location = 0) out vec4 pos;
layout(location = 1) out float vb;
layout(location = 8) out float colorOffset;

uniform float dy;

void main()
{
  float Y = Position.y + sin( (Position.x + dy)*5)/2;
  colorOffset = dy;
  gl_Position = vec4(Position.x, Y ,Position.z,1.0);
  
  pos = vec4(Position.x, Y ,Position.z,1.0);
  gl_PointSize = 15.0;
  
  vb = sin( (Bright + dy)*5);
}