//-----------------------------------
// OpenGL 3.x context creator
//-----------------------------------
#include "GLWindow.h"



int main()
{
	int      result;

	if (!GLWindowCreate("lesson01", 640, 480, false))
		return 1;
	
     GLint max_patchv = glGetError();
     glGetIntegerv(GL_MAX_PATCH_VERTICES,&max_patchv);
     printf("GL_MAX_PATCH_VERTICES %i\n",  max_patchv);
     
     time_t startTime = time(NULL);
     long totalFrame(0);
     
     totalFrame = GLWindowMainLoop();
     
     printf("Average FPS %f\n", totalFrame/(0.00001+(time(NULL)-startTime)));
	
    GLWindowDestroy();

	return 0;


}