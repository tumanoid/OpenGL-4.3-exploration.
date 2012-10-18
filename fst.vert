#version 430 core

in vec3 Position;
in float Bright;

out float vb;

uniform float dy;

void main()
{
  float Y = Position.y + sin( (Position.x + dy)*5)/2;
  gl_Position = vec4(Position.x, Y ,Position.z,1.0);
  gl_PointSize = 50.0;
  vb = sin( (Bright + dy)*5);
}
