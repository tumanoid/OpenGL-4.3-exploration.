#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in float Bright;

layout(location = 1) out vec3 vb;

uniform mat4 MVP;
uniform float dy;

void main()
{
  //float Y = Position.y + 0.3*sin( (Position.x + dy)*5)/2;/
	
  //gl_Position = MVP * vec4(Position.x, Y ,Position.z,1.0);/
  
  gl_Position = MVP * vec4(Position,1.0);

  //gl_PointSize = 15.0;/
  
  vb = Normal;
}
