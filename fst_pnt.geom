#version 430 core

layout(points) in;
layout(points, invocations = 5) in;
layout(points, max_vertices=15) out;

layout(location = 8) in float colorOffset[];

layout(location = 5) out float nerta;

void main()
{
  
  float radius = 0.2/(gl_InvocationID+0.0001);
  
  vec4 bb = gl_in[0].gl_Position;
  for ( int i = 0; i < 15; i++ )
  {
	gl_Position = vec4(bb.x+radius*sin(i/15.0*3.14*2.0), bb.y+radius*cos(i/15.0*3.14*2.0), 1,1);
  	gl_PointSize = 5;
        
	nerta = sin(colorOffset[0]*5)*(i+0.001)/16.0;
    EmitVertex ();  	
  }
	
  EndPrimitive ();
}