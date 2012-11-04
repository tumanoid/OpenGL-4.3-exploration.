#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include "common.h"

class RenderMesh
{
public:
    RenderMesh();
    ~RenderMesh();
    
    void Create(float* vv, DWORD* ff, DWORD cv, DWORD cf);
    void PushGL();
    void Draw(matrix4f cameraView);
    void PopGL();

    void CreateShader();//FIXIT. there is material system

public:
    GLuint countVertex;
    GLuint countFace;

    GLuint vertexVBO;
    GLuint indexVBO;
    GLuint meshVAO;

    GLuint PipelineName;//shader
    
    GLint attrPosition;
    GLint attrUV;
    GLint attrNormal;
    GLint attrBright;
    GLint attribModelView;

    vec3f           position;
    quaternionf     rotation;
    matrix4f        ModelView;

    float* vertex; // point/color
    DWORD* face;
};


RenderMesh::RenderMesh(): vertex(NULL), face(NULL), vertexVBO(0),indexVBO(0),meshVAO(0)
{
    position = vec3f(0,0,0);
    rotation = quaternionf(0,0,0,1);
    ModelView.make_identity();
};


void RenderMesh::Create(float* vv, DWORD* ff, DWORD cv, DWORD cf)
{
    countVertex = cv;
    countFace   = cf;

    vertex = new float[countVertex*(3+3+2+1)];
    face   = new DWORD[countFace*3];
    
    memcpy(vertex, vv, sizeof(float)*(3+3+2+1)*cv);
    memcpy(face, ff,   sizeof(DWORD)*3*cf);
}


void RenderMesh::PushGL()
{
    if ( !vertex || !face ) return;
    
    
    //-------------------------
    // Shader attribs
    //-------------------------
    CreateShader(); // create PipeLine. FIXIT: not here

    GLint vertProgram(0);
    glGetProgramPipelineiv(PipelineName,GL_VERTEX_SHADER, &vertProgram);
    
    // attribs
    attrPosition = glGetAttribLocation(vertProgram, "Position");
    attrNormal   = glGetAttribLocation(vertProgram, "Normal");
    attrUV       = glGetAttribLocation(vertProgram, "UV");
    attrBright   = glGetAttribLocation(vertProgram, "Bright");
    // uniform
    attribModelView = glGetUniformLocation(vertProgram, "MVP");
    
    
    //-------------------------
    // VAO
    //-------------------------
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO); 
    
    
    //-------------------------
    // VBO
    //-------------------------
        // vertex
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, countVertex*sizeof(GLfloat)*(3+3+2+1), vertex, GL_STATIC_DRAW);
    
    glVertexAttribPointer(attrPosition, 3, GL_FLOAT, GL_FALSE, 36, 0);
    glEnableVertexAttribArray(attrPosition);
    
    glVertexAttribPointer(attrNormal, 3, GL_FLOAT, GL_FALSE, 36, (const void*)12);
    glEnableVertexAttribArray(attrNormal);
    
    glVertexAttribPointer(attrUV, 2, GL_FLOAT, GL_FALSE, 36, (const void*)24);
    glEnableVertexAttribArray(attrUV);

    glVertexAttribPointer(attrBright, 1, GL_FLOAT, GL_FALSE, 36, (const void*)32);
    glEnableVertexAttribArray(attrBright); 
        
        // index
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, countFace*sizeof(unsigned int)*3, face, GL_STATIC_DRAW);

    delete [] vertex;
    delete [] face;
}


void RenderMesh::PopGL()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &meshVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &indexVBO);
}


void RenderMesh::Draw(matrix4f cameraView)
{
    glBindProgramPipeline(PipelineName);

    GLint vertProgram(0);
    glGetProgramPipelineiv(PipelineName,GL_VERTEX_SHADER, &vertProgram);
    
    ModelView.make_identity();
    ModelView*=cameraView;

    glProgramUniformMatrix4fv(vertProgram, attribModelView, 1, GL_FALSE, ModelView._array);
    
	glBindVertexArray(meshVAO);
    glDrawElements(GL_TRIANGLES, countFace*3, GL_UNSIGNED_INT, NULL);
}


RenderMesh::~RenderMesh()
{  
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &meshVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &indexVBO);
}


void RenderMesh::CreateShader()
{

    GLint vertexShader   = glCreateShader(GL_VERTEX_SHADER);  
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    

    //-------------------------------
    // VERTEX SHADER
    //-------------------------------
    ReadSrcFile  vertFile("fst_tetra.vert");
    char const * srcv = vertFile.GetSrc();
    
    glShaderSource(vertexShader, 1, &srcv , NULL);
    glCompileShader(vertexShader);


    //-------------------------------
    // FRAGMENT SHADER
    //-------------------------------
    ReadSrcFile  fragFile("fst_tetra.frag");
    char const * srcf = fragFile.GetSrc();

    glShaderSource(fragmentShader, 1, &srcf, NULL);
    glCompileShader(fragmentShader);


    //-------------------------------
    // GEOMETRY SHADER
    //-------------------------------
    ReadSrcFile  geomFile("fst_tetra.geom");
    char const * srcg = geomFile.GetSrc();

    glShaderSource(geometryShader, 1, &srcg, NULL);
    glCompileShader(geometryShader);


    //------------------
    // Create PipeLine
    //------------------
    glGenProgramPipelines(1, &PipelineName);

    GLuint vertProgram = glCreateProgram();
    GLuint geomProgram = glCreateProgram();
    GLuint fragProgram = glCreateProgram();

    glProgramParameteri(vertProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glProgramParameteri(geomProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glProgramParameteri(fragProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    
    glAttachShader(vertProgram, vertexShader);
    glAttachShader(geomProgram, geometryShader);
    glAttachShader(fragProgram, fragmentShader);
    
    glLinkProgram(vertProgram);
    glLinkProgram(geomProgram);
    glLinkProgram(fragProgram);

    glDeleteShader(vertexShader);    
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);

    glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT , vertProgram);
   // OPENGL_CALL( glUseProgramStages(PipelineName, GL_GEOMETRY_SHADER_BIT, geomProgram) );
    glUseProgramStages(PipelineName, GL_FRAGMENT_SHADER_BIT, fragProgram);
}


#endif//RENDER_MESH_H