#ifndef COMMON_H
#define COMMON_H

#define XRES 1280
#define YRES 1024
#define ASPECT (float)XRES/YRES

#define  _CRT_SECURE_NO_WARNINGS

#include	<windows.h>

#include	<gl/glew.h>
#include	<gl/wglew.h>

#include	<stdio.h>
#include	<stdlib.h>
#include	<string>
#include	<vector>

// NVidia math library
#include <nvMath.h>
using namespace  nv;

// for FPS
#include	<time.h>
#include    <mmsystem.h>

using namespace  std;

// глобальная перменная для хранения ошибки OpenGL
extern GLenum g_OpenGLError;

// проверка на ошибки OpenGL
#define OPENGL_CHECK_ERR(msg) \
    {\
        if ((g_OpenGLError = glGetError()) != GL_NO_ERROR) \
        printf(#msg"OpenGL error %X\n", (int)g_OpenGLError);\
    }

// безопасный вызов функции OpenGL
#define OPENGL_CALL(expression) \
	{ \
		expression; \
		if ((g_OpenGLError = glGetError()) != GL_NO_ERROR) \
			printf("OpenGL expression \"" #expression "\" error %X\n", (int)g_OpenGLError); \
	}


struct ReadSrcFile {
private:
    ReadSrcFile(){}

public:
    ReadSrcFile(char* fName):buff(NULL), charCount(0)
    {
      FILE* f;
      f = fopen(fName,"r");
        if (!f) { printf("File %s not found.\n", fName); return; }

      while (!feof(f))
        {
          getc(f);
          charCount++;
        }

      buff = new char[charCount];
      memset(buff,0,charCount);

      fseek(f,0,SEEK_SET);
      fread(buff, 1, charCount, f);
      fclose(f);
    }
    ~ReadSrcFile() { delete[] buff; }

    char const * GetSrc() { return buff; }
    int   GetLength() { return charCount; }

    char* buff;

private:
    int   charCount;
};


#endif//COMMON_H

/*
DWORD wait_GetInput(0), delta_GetInput(1);
if ( (timeGetTime() - wait_GetInput)< delta_GetInput ) return;
wait_GetInput = timeGetTime();
*/