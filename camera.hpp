#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "input.hpp"

void AngleVectors (vec3f angles, vec3f &forward, vec3f &right, vec3f &up);

matrix4f PerspectiveMatrix(float fovy, float aspect, float znear, float zfar)
{

        float f = 1 / tanf(fovy * NV_PI / 360);
        float A = (zfar + znear) / (znear - zfar);
        float B = (2 * zfar * znear) / (znear - zfar);
/*
        matrix4f M(f / aspect,0,0,0,
                   0,f,0,0,
                   0,0,A,B,
                   0,0,-1,0);
*/
        matrix4f M(f / aspect,0,0,0,
                   0,f,0,0,
                   0,0,A,-1,
                   0,0,B,0);
        return M;
}

class camera
{
public:    
    matrix4f V, P, R; // view and projection matrix
    
    vec3f position;
    vec3f euler_rotation;

    quaternionf rotation;
    
    float FOV, Aspect;
    float zNear, zFar;
    
    camera()
    {      
        V.make_identity();
        P.make_identity();
        R.make_identity();               
        position.set_value( vec3f(0,0,100) );
        euler_rotation.set_value( vec3f(0,0,0) );
        rotation.set_value( vec3f(0,1,0), 0);
    }
    
    void SetPosition(vec3f pos) { position = pos; }
    void SetEulerAngles(vec3f ang) { euler_rotation = ang; }
    void SetRotation(quaternionf q) { rotation = q; }

    matrix4f VP()
    {
        V = PerspectiveMatrix(float(50.0), float((XRES+0.1)/(YRES+0.1)), float(1), float(1000) );
        
        P.set_translate( vec3f( -position.x ,-position.y ,-position.z ) );        

        rotation.get_value(R);
        
        return V*R*P;
        
    }
    
    void Update()
    {
        const float t_sens=float(0.1);
        const float r_sens=float(0.1);
                
        euler_rotation.y+=input::mouseX;
        euler_rotation.x+=input::mouseY;

        rotation = quaternionf(vec3f(1,0,0),r_sens*euler_rotation.x*NV_PI/180);
        rotation*= quaternionf(vec3f(0,1,0),r_sens*euler_rotation.y*NV_PI/180);
        
        vec3f l_right(1,0,0);
        vec3f l_forward(0,0,1);

        quaternionf rev = inverse(rotation);
        rev.mult_vec(l_forward);
        rev.mult_vec(l_right);

        position-= (t_sens * (input::Forward-input::Backward))*l_forward;
        position-= (t_sens * (input::LeftStrafe-input::RightStrafe))*l_right;

    }

    ~camera(){}
};

camera main_camera;


void AngleVectors (vec3f angles, vec3f &left, vec3f &up, vec3f &forward)
{
    const float DEG2RAD = NV_PI / 180;
    float sx, sy, sz, cx, cy, cz, theta;

    // rotation angle about X-axis (pitch)
    theta = angles.x * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);

    // rotation angle about Y-axis (yaw)
    theta = angles.y * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);

    // rotation angle about Z-axis (roll)
    theta = angles.z * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);

    // determine left axis
    left.x = cy*cz;
    left.y = sx*sy*cz + cx*sz;
    left.z = -cx*sy*cz + sx*sz;

    // determine up axis
    up.x = -cy*sz;
    up.y = -sx*sy*sz + cx*cz;
    up.z = cx*sy*sz + sx*cz;

    // determine forward axis
    forward.x = sy;
    forward.y = -sx*cy;
    forward.z = cx*cy;
}
#endif//CAMERA_H