//-----------------------------------
// OpenGL 3.x context creator
//-----------------------------------
#include "GLWindow.h"

int main()
{
	int      result;

	if (!GLWindowCreate("lesson01", 640, 480, false))
		return 1;
	
     //GLenum glErr = glGetError();
     //if (glErr!=GL_NO_ERROR) printf("GLWindowCreate GL error: %X", glErr);
     
     result = GLWindowMainLoop();

	GLWindowDestroy();

	return result;


}