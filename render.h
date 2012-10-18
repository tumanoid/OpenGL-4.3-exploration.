#ifndef RENDER_H
#define RENDER_H

#include "common.h"
#include "obj_reader.h"

GLenum g_OpenGLError;

GLuint meshVBO;
GLuint meshVAO;

GLint stat;
// shader
GLint shdrProgram = 0;
GLuint AttribPosition;

static bool    isInitDone = false;

obj_t GO;
//-------------------
// vertex shader
//-------------------
const char* vertexSRC[]={"                  \
    \
    in vec3 Position;                       \
    void main() {                           \
        gl_Position = vec4(Position,1.0);   \
    }"                          
};

//-------------------
// fragment shader
//-------------------
/*const char* fragmentSRC[]={"\
void main() {\
gl_FragColor = vec4(0,1,0,1);\
}"
};*/

const char* fragmentSRC[]={"\
#define FRAG_COLOR	0\
void main() {\
}"
};

//#version 430 core\
//gl_FragColor = vec4(0,0,1,0);\
//#define FRAG_COLOR	0\
//layout(location = FRAG_COLOR, index = 0) out vec4 Color;\

void GetShaderProgram()
{

    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        OPENGL_CHECK_ERR("glCreateShader GL_VERTEX_SHADER");
    
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        OPENGL_CHECK_ERR("glCreateShader GL_FRAGMENT_SHADER");

    
    ReadSrcFile  vertFile("fst.vert");
    char const * src = vertFile.GetSrc();
    
    //printf(">VERTEX SHADER\n%s\n>ENDSHADER\n",&src);
    OPENGL_CALL( glShaderSource(vertexShader, 1, &src , NULL) );
    OPENGL_CALL( glCompileShader(vertexShader) );
    //--------------------------------
    // VERTEX Compile err log
    //--------------------------------
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&stat);
        printf("VT GL_COMPILE_STATUS %X\n", stat);
    
    GLint srcLength;
    glGetShaderiv(vertexShader,GL_SHADER_SOURCE_LENGTH,&srcLength);
        printf("VT GL_SHADER_SOURCE_LENGTH %i\n", srcLength);
    
    char* a = new char[srcLength+1];
    glGetShaderInfoLog(vertexShader, srcLength, NULL, a);
        printf("VT >%s\n", a); delete[] a;
    //--------------------------------

    ReadSrcFile  fragFile("fst.frag");
    src = fragFile.GetSrc();

    OPENGL_CALL( glShaderSource(fragmentShader, 1, &src, NULL) );
    OPENGL_CALL( glCompileShader(fragmentShader) );
    //--------------------------------
    // FRAGMENT Compile err log
    //--------------------------------
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&stat);
        printf("FR GL_COMPILE_STATUS %X\n", stat);
    
    srcLength;
    glGetShaderiv(vertexShader,GL_SHADER_SOURCE_LENGTH,&srcLength);
        printf("FR GL_SHADER_SOURCE_LENGTH %i\n", srcLength);
    
    a = new char[srcLength+1];
    glGetShaderInfoLog(vertexShader, srcLength, NULL, a);
        printf("FR >%s\n", a); delete[] a;
    //--------------------------------


    shdrProgram = glCreateProgram();
        OPENGL_CHECK_ERR("glCreateProgram");
    
    OPENGL_CALL( glAttachShader(shdrProgram, vertexShader) );
    OPENGL_CALL( glAttachShader(shdrProgram, fragmentShader) );
    
    OPENGL_CALL( glDeleteShader(vertexShader) );
    OPENGL_CALL( glDeleteShader(fragmentShader) );

    OPENGL_CALL( glLinkProgram(shdrProgram) );

    AttribPosition = glGetAttribLocation(shdrProgram, "Position");
    
}

void Init()
{
    GO = ObjRead("cubes_small.obj");    
    //GO = ObjRead("SpheraSky.obj");   
    
    GetShaderProgram();
    //-------------------------
    // VBO
    //-------------------------
    glGenBuffers(1, &meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, GO.cVertex*sizeof(GLfloat)*3, GO.vtx, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);  

    //-------------------------
    // VAO
    //-------------------------
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glVertexAttribPointer(AttribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(AttribPosition);
   
    glBindVertexArray(0);

    isInitDone = true;
}

void Render()
{
     if (!isInitDone) Init();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
     

    OPENGL_CALL( glUseProgram(shdrProgram) );
      
	OPENGL_CALL(glBindVertexArray(meshVAO));
        OPENGL_CALL( glDrawArrays(GL_TRIANGLES , 0, 21 ));//GO.cVertex) );
    OPENGL_CALL(glBindVertexArray(0));

    OPENGL_CALL( glUseProgram(0) );


}
#endif//RENDER_H