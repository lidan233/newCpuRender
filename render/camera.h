
// Created by 李源 on 2020-12-18.
//

#ifndef NEWCPURENDER_CAMERA_H
#define NEWCPURENDER_CAMERA_H
#include "Vec.h"
#include "manipulation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum NCamera_Movement {
    ZERO,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float NYAW         = 180.0f;
const float NPITCH       =  0.0f;
const float NSPEED       =  2.5f;
const float NSENSITIVITY =  2.0f;
const float NZOOM        =  45.0f;


template <typename genType>
genType radians(genType degrees)
{
    return degrees * static_cast<genType>(0.01745329251994329576923690768489);
}


class camera {

private:
    // camera Attributes
    Vec3f Position;
    Vec3f Front;
    Vec3f Up;
    Vec3f Right;
    Vec3f WorldUp;
    Vec3f Target ;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float radius ;

public:
    camera( Vec3f position = Vec3f(0.0f, 0.0f, 3.0f),
            Vec3f up = Vec3f(0.0f, 1.0f, 0.0f),
            Vec3f target = Vec3f(0.0,0.0,0.0) ,
    float yaw = NYAW,
    float pitch = NPITCH)
    :MovementSpeed(NSPEED),
    MouseSensitivity(NSENSITIVITY),
    Zoom(NZOOM)
    {
        Front = (target-position).normalize() ;
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Target = target ;
        radius = (Position-Target).norm() ;

        updateCameraVectors();
    }

    void setCenter(Vec3f center)
    {
        Vec3f dir = (center-Target).normalize() ;
        Target = center ;
        Position = Target + radius*dir ;
        Front = (Target-Position).normalize() ;
    }
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    Matrix44 GetViewMatrix()
    {
        return manipulation::lookAt(Position, Position + Front, Up);
    }



    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(NCamera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position = Position + Front * velocity;
        if (direction == BACKWARD)
            Position = Position -  Front * velocity;
        if (direction == LEFT)
            Position = Position - Right * velocity;
        if (direction == RIGHT)
            Position = Position + Right * velocity;

        radius = (Target - Position).norm() ;
//        log(INFO," " +std::to_string(Position[0]) +" " +std::to_string(Position[1])+" " +std::to_string(Position[2]));
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   -= yoffset;
        Pitch += xoffset;

        updateCameraVectors();
    }


    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    float getZoom(){ return Zoom; }

private:

    void updateCameraVectors()
    {
        // calculate the new Front vector
        Vec3f front;
        front.x = cos(radians(Yaw)) * cos(radians(Pitch));
        front.y = sin(radians(Pitch));
        front.z = sin(radians(Yaw)) * cos(radians(Pitch));
        Front = (front).normalize() ;
        Right = (cross(Front, WorldUp)).normalize() ;  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = (cross(Right, Front)).normalize() ;
        Position = Target - radius * Front ;
    }

};


#endif //NEWCPURENDER_CAMERA_H
