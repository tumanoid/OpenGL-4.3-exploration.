//-----------------------------------
// OpenGL 4.x context creator
//-----------------------------------
#include "GLWindow.h"

int main()
{
	
	if (!GLWindowCreate("Space Fermanium", XRES, YRES, false))
		return 1;
	
     GLint varLog = glGetError();
     
     glGetIntegerv(GL_MAX_PATCH_VERTICES,&varLog);
     printf("GL_MAX_PATCH_VERTICES %i\n",  varLog);
     
     glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES,&varLog);
     printf("MAX_GEOMETRY_OUTPUT_VERTICES %i\n",  varLog);
     

     GLWindowMainLoop();
     
     GLWindowDestroy();

	return 0;


}