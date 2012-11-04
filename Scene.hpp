#ifndef RENDER_SCENE_HPP
#define RENDER_SCENE_HPP

#include "common.h"
#include "obj_reader.h"
#include "RenderMesh.hpp"
#include "camera.hpp"

namespace scene
{

RenderMesh  go;

void Init()
{
    objNode mesh;
    mesh.ObjRead("cubes_small.model");
    
    go.Create(mesh.vtx, mesh.face, mesh.countVertex, mesh.countFace);
    go.PushGL();

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

}
    
void Render()
{
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5,0.6,1.0,1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO); 

    go.Draw( main_camera.VP() );
}


}
#endif//RENDER_SCENE_HPP