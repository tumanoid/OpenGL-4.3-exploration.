#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "camera.hpp"
#include "input.hpp"
#include "scene.hpp"

namespace game
{
    void Init()
    {
        scene::Init();
    }

    void Update()
    {
        input::GetInput();
        
        main_camera.Update();
        
        scene::Render();
    }

}//namespace GAME
#endif//GAME_H