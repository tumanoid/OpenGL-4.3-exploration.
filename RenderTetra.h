#ifndef RENDER_TETRA_H
#define RENDER_TETRA_H

#include "common.h"
#include "obj_reader.h"
#include "camera.hpp"

namespace ttr {

GLenum g_OpenGLError;

GLuint meshVBO;
GLuint indexVBO;
GLuint meshVAO;

GLint stat;

// shader
GLuint shdrProgram(0);
GLuint PipelineName(0);

//-----------------------------
// Attributes and Uniforms
//-----------------------------
GLint attrPosition = -1;
GLint attrBright = -1;
GLint MVP = -1;
GLint paramDY = -1;
GLint paramColor = -1;

int countElement = 0;

static bool    isInitDone = false;

inline bool checkProgram(GLuint ProgramName)
{
		if(!ProgramName)
			return false;

		GLint Result = GL_FALSE;
		glGetProgramiv(ProgramName, GL_LINK_STATUS, &Result);

		fprintf(stdout, "Linking program\n");
		int InfoLogLength;
		glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);

		return Result == GL_TRUE;
}

void GetShaderProgram()
{
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        OPENGL_CHECK_ERR("glCreateShader GL_VERTEX_SHADER");
    
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        OPENGL_CHECK_ERR("glCreateShader GL_FRAGMENT_SHADER");
    
    GLint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        OPENGL_CHECK_ERR("glCreateShader GL_GEOMETRY_SHADER");

    //-------------------------------
    // VERTEX SHADER
    //-------------------------------
    
    ReadSrcFile  vertFile("fst_tetra.vert");
    char const * srcv = vertFile.GetSrc();
    
    OPENGL_CALL( glShaderSource(vertexShader, 1, &srcv , NULL) );
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


    //-------------------------------
    // FRAGMENT SHADER
    //-------------------------------
    ReadSrcFile  fragFile("fst_tetra.frag");
    char const * srcf = fragFile.GetSrc();

    OPENGL_CALL( glShaderSource(fragmentShader, 1, &srcf, NULL) );
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

    //-------------------------------
    // GEOMETRY SHADER
    //-------------------------------
    ReadSrcFile  geomFile("fst_tetra.geom");
    char const * srcg = geomFile.GetSrc();

    OPENGL_CALL( glShaderSource(geometryShader, 1, &srcg, NULL) );
    OPENGL_CALL( glCompileShader(geometryShader) );
    //--------------------------------
    // GEOMETRY Compile err log
    //--------------------------------
        glGetShaderiv(geometryShader,GL_COMPILE_STATUS,&stat);
            printf("GEO GL_COMPILE_STATUS %X\n", stat);
    
        srcLength;
        glGetShaderiv(geometryShader,GL_SHADER_SOURCE_LENGTH,&srcLength);
            printf("GEO GL_SHADER_SOURCE_LENGTH %i\n", srcLength);
    
        a = new char[srcLength+1];
        glGetShaderInfoLog(geometryShader, srcLength, NULL, a);
            printf("GEO >%s\n", a); delete[] a;
/*
    //Section for solid shader program

    shdrProgram = glCreateProgram();
       OPENGL_CHECK_ERR("glCreateProgram");

    OPENGL_CALL( glAttachShader(shdrProgram, vertexShader) );
    OPENGL_CALL( glAttachShader(shdrProgram, geometryShader) );
    OPENGL_CALL( glAttachShader(shdrProgram, fragmentShader) );
   
    OPENGL_CALL( glLinkProgram(shdrProgram) );
        OPENGL_CHECK_ERR("glLinkProgram");

    // Get attributes and uniforms
   // OPENGL_CALL( glUseProgram(shdrProgram) );

    attrPosition = glGetAttribLocation(shdrProgram, "Position");
    attrBright   = glGetAttribLocation(shdrProgram, "Bright");

    //paramDY = glGetUniformLocation(shdrProgram, "dy");
    //paramColor = glGetUniformLocation(shdrProgram, "rc");

    OPENGL_CALL( glDeleteShader(vertexShader) );    
    OPENGL_CALL( glDeleteShader(geometryShader) );
    OPENGL_CALL( glDeleteShader(fragmentShader) );*/
    
    //------------------
    // Create PipeLine
    //------------------
    OPENGL_CALL( glGenProgramPipelines(1, &PipelineName) );

    GLuint vertProgram = glCreateProgram();
    GLuint geomProgram = glCreateProgram();
    GLuint fragProgram = glCreateProgram();

    OPENGL_CALL( glProgramParameteri(vertProgram, GL_PROGRAM_SEPARABLE, GL_TRUE) );
    OPENGL_CALL( glProgramParameteri(geomProgram, GL_PROGRAM_SEPARABLE, GL_TRUE) );
    OPENGL_CALL( glProgramParameteri(fragProgram, GL_PROGRAM_SEPARABLE, GL_TRUE) );
    
    OPENGL_CALL( glAttachShader(vertProgram, vertexShader) );
    OPENGL_CALL( glAttachShader(geomProgram, geometryShader) );
    OPENGL_CALL( glAttachShader(fragProgram, fragmentShader) );
    
    checkProgram(vertProgram);
    checkProgram(geomProgram);
    checkProgram(fragProgram);

    OPENGL_CALL( glLinkProgram(vertProgram) );
    OPENGL_CALL( glLinkProgram(geomProgram) );
    OPENGL_CALL( glLinkProgram(fragProgram) );

    OPENGL_CALL( glDeleteShader(vertexShader) );    
    OPENGL_CALL( glDeleteShader(geometryShader) );
    OPENGL_CALL( glDeleteShader(fragmentShader) );

    OPENGL_CALL( glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT , vertProgram) );
   // OPENGL_CALL( glUseProgramStages(PipelineName, GL_GEOMETRY_SHADER_BIT, geomProgram) );
    OPENGL_CALL( glUseProgramStages(PipelineName, GL_FRAGMENT_SHADER_BIT, fragProgram) );

                GLint Status(0);
                GLint LengthMax(0);
                
                glValidateProgramPipeline(PipelineName);
                glGetProgramPipelineiv(PipelineName, GL_VALIDATE_STATUS, &Status);
                glGetProgramPipelineiv(PipelineName, GL_INFO_LOG_LENGTH, &LengthMax);
                printf("PipeLine status is: %i\n", Status);
                
                char* str = new char[LengthMax]; 
                GLsizei tlb;
                glGetProgramPipelineInfoLog(PipelineName,LengthMax, &tlb, str);
                delete [] str;
                
                printf("PipeLine LOG:\n%s\n", str);

    attrPosition = glGetAttribLocation(vertProgram, "Position");
    printf("attrPosition %i \n", attrPosition);

    attrBright = glGetAttribLocation(vertProgram, "Bright");
    printf("Bright %i \n", attrBright);
    
    MVP = glGetUniformLocation(vertProgram, "MVP");
    printf("MVP %i \n", MVP);

    paramDY = glGetUniformLocation(vertProgram, "dy");
    printf("paramDY %i \n", paramDY);
    
    OPENGL_CALL( glBindProgramPipeline(PipelineName) );

}

void Init()
{
    objNode GO;
    GO.ObjRead("cubes_small.model");
    countElement = GO.countFace;
    if (!GO.countVertex) { printf("ERR: Zero Vertex count\n"); return; }
    if (!GO.vtx) { printf("ERR: Zero Vertex pinter\n"); return; }
    if (!GO.countFace) { printf("ERR: Zero Face count\n"); return; }
    if (!GO.face) { printf("ERR: Zero Face pinter\n"); return; }
    
    //-------------------------
    // PipeLine program
    //-------------------------
    GetShaderProgram();

    //-------------------------
    // VAO
    //-------------------------
glGenVertexArrays(1, &meshVAO);
glBindVertexArray(meshVAO); 
    
    //-------------------------
    // VBO
    //-------------------------
        // vertex
    glGenBuffers(1, &meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, GO.countVertex*sizeof(GLfloat)*4, GO.vtx, GL_STATIC_DRAW);
    
    glVertexAttribPointer(attrPosition, 3, GL_FLOAT, GL_FALSE, 16, 0);
    glEnableVertexAttribArray(attrPosition);

    glVertexAttribPointer(attrBright, 1, GL_FLOAT, GL_FALSE, 16, (const void*)12);
    glEnableVertexAttribArray(attrBright); 
        
        // index
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GO.countFace*sizeof(unsigned int)*3, GO.face, GL_STATIC_DRAW);


    isInitDone = true;
}

float offsetY=0;
vec3f cos_rotation(0,0,0);
vec3f position(0,0,0);

void Render()
{
     if (!isInitDone) Init();
     
     if (!isInitDone) return;
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
     
   // OPENGL_CALL( glUseProgram(shdrProgram) );
      

    //GLint vp(0);
    //OPENGL_CALL( glGetProgramPipelineiv(PipelineName,GL_VERTEX_SHADER, &vp));
    //OPENGL_CALL( glProgramUniform1f (vp, paramDY, offsetY));
   //     glUniform1f( paramColor, (rand() % 100)/99.9999f);
   //     glUniform1f( paramDY, offsetY);
    
    //--------------------------------
    // Set matrix
    //--------------------------------
    offsetY+=0.01f;

    //matrix4f F = PerspectiveMatrix(float(40.0), float((XRES+0.1)/(YRES+0.1)), 1, 600);
    //matrix4f M; 
    //M.set_translate( vec3f(0,0,-240) );
    //M.set_scale( vec3f(1,1,1) );

    quaternionf rotation(vec3f(0,1,0), offsetY*NV_PI/180);    
    
    matrix4f R;
    rotation.get_value(R);
   
    matrix4f M; 
    M.set_translate( vec3f(0,0,0) );

    matrix4f OutMVP = main_camera.VP()*M*R; 

    //--------------------------------
    // Draw
    //--------------------------------
    OPENGL_CALL( glBindProgramPipeline(PipelineName) );

    GLint vertPrg(0);
    OPENGL_CALL( glGetProgramPipelineiv(PipelineName,GL_VERTEX_SHADER, &vertPrg));
    OPENGL_CALL( glProgramUniformMatrix4fv(vertPrg, MVP, 1, GL_FALSE, OutMVP._array) );
    OPENGL_CALL( glProgramUniform1f (vertPrg, paramDY, offsetY));

	OPENGL_CALL( glBindVertexArray(meshVAO) );

      //  OPENGL_CALL( glDrawArrays(GL_TRIANGLES , 0, 500 ));
    OPENGL_CALL( glDrawElements(GL_TRIANGLES, countElement*3, GL_UNSIGNED_INT, NULL) );

}
}//end name space TTR
#endif//RENDER_TETRA_H