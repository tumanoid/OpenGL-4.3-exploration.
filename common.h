#ifndef COMMON_H
#define COMMON_H

#define  _CRT_SECURE_NO_WARNINGS

#include	<windows.h>


//#include	<gl/glcorearb.h>
#include	<gl/glew.h>
#include	<gl/wglew.h>

#include	<stdio.h>
#include	<stdlib.h>
#include	<string>

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
    ReadSrcFile(char* fName):buff(0), charCount(0)
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
    ~ReadSrcFile() { printf(">>>>>del buff\n"); delete[] buff; }

    char const * GetSrc() { /*printf(">SHADER\n%s\n>ENDSHADER\n",buff); */return buff; }
    int   GetLength() { return charCount; }

    char* buff;

private:
    int   charCount;
};


#endif//COMMON_H