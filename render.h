#ifndef RENDER_H
#define RENDER_H

#include "common.h"
#include "obj_reader.h"

GLenum g_OpenGLError;

GLuint meshVBO;
GLuint meshVAO;

GLint stat;
// shader
GLuint  shdrProgram(0);
GLuint PipelineName(0);

//-----------------------------
// Attributes and Uniforms
//-----------------------------
GLint attrPosition = -1;
GLint attrBright = -1;

GLint paramDY = -1;
GLint paramColor = -1;

static bool    isInitDone = false;

obj_t GO;

inline bool checkProgram(GLuint ProgramName)
{
		if(!ProgramName)
			return false;

		GLint Result = GL_FALSE;
		glGetProgramiv(ProgramName, GL_LINK_STATUS, &Result);

		fprintf(stdout, "Linking program\n");
		int InfoLogLength;
		glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);
		vector<char> Buffer(max(InfoLogLength, int(1)));
		glGetProgramInfoLog(ProgramName, InfoLogLength, NULL, &Buffer[0]);
		fprintf(stdout, "%s\n", &Buffer[0]);

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
    
    ReadSrcFile  vertFile("fst.vert");
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
    // VERTEX SHADER
    //-------------------------------
    ReadSrcFile  fragFile("fst.frag");
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
    ReadSrcFile  geomFile("fst.geom");
    char const * srcg = geomFile.GetSrc();

    OPENGL_CALL( glShaderSource(geometryShader, 1, &srcg, NULL) );
    OPENGL_CALL( glCompileShader(geometryShader) );
    //--------------------------------
    // FRAGMENT Compile err log
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

    // Create PipeLine
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
    OPENGL_CALL( glUseProgramStages(PipelineName, GL_GEOMETRY_SHADER_BIT, geomProgram) );
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
                
                printf("PipeLine LOG:\n%s", str);

    attrPosition = glGetAttribLocation(vertProgram, "Position");
    printf("attrPosition %i \n", attrPosition);
    
    attrBright = glGetAttribLocation(vertProgram, "Bright");
    printf("attrBright %i \n", attrBright);
    
    paramDY = glGetUniformLocation(vertProgram, "dy");
     printf("paramDY %i \n", paramDY);
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
    glBufferData(GL_ARRAY_BUFFER, GO.cVertex*sizeof(GLfloat)*4, GO.vtx, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);  
    
    //-------------------------
    // VAO
    //-------------------------
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        
        glVertexAttribPointer(attrPosition, 3, GL_FLOAT, GL_FALSE, 16, 0);
        glEnableVertexAttribArray(attrPosition);

        glVertexAttribPointer(attrBright, 1, GL_FLOAT, GL_FALSE, 16, (const GLvoid*)12);
        glEnableVertexAttribArray(attrBright);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    isInitDone = true;
}

float offsetY=0;

void Render()
{
     if (!isInitDone) Init();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
     
   // OPENGL_CALL( glUseProgram(shdrProgram) );
    OPENGL_CALL( glBindProgramPipeline(PipelineName) );
    
    GLint vp(0);
    glGetProgramPipelineiv(PipelineName,GL_VERTEX_SHADER, &vp);
    glProgramUniform1f (vp, paramDY, offsetY);
   //     glUniform1f( paramColor, (rand() % 100)/99.9999f);
   //     glUniform1f( paramDY, offsetY);
        offsetY+=0.01f;

	OPENGL_CALL(glBindVertexArray(meshVAO));
        OPENGL_CALL( glDrawArrays(GL_POINTS , 0, 50 ));
    OPENGL_CALL(glBindVertexArray(0));
    
   // OPENGL_CALL( glBindProgramPipeline(0) );
    //OPENGL_CALL( glUseProgram(0) );


}
#endif//RENDER_H