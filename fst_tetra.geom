#version 430 core

layout(points) in;
layout(points, max_vertices=300) out;

void main()
{
  
  vec4 bb = gl_in[0].gl_Position;
  for ( int i = 1; i < 200; i++ )
  {
	gl_Position = vec4(bb.x+sin(3.14/i), bb.y+cos(3.14/i), 1,1);
  	gl_PointSize = 5;
        EmitVertex ();
  	EndPrimitive ();
  }


  vec4 a =gl_in[0].gl_Position;
  a.x = a.x+0.1;
  gl_Position = a;
  gl_PointSize = 5;
  EmitVertex ();
  EndPrimitive ();
  
  a.x = a.x-0.2;
  gl_Position = a;
  gl_PointSize = 10;
  EmitVertex ();
  EndPrimitive ();		
  
  a.y = a.y+0.1;
  gl_Position = a;
  gl_PointSize = 30;
  EmitVertex ();
  EndPrimitive ();		
}