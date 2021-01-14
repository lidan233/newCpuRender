//
// Created by 李源 on 2019-11-29.
//

#ifndef TINYRENDER_MANIPULATION_H
#define TINYRENDER_MANIPULATION_H


#include "Vec.h"
#include <cmath>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

class manipulation {

public:
    static Vec3f m2v(Matrix41 m) ;
    static Matrix41 v2m(Vec3f v) ;
    static Matrix44 viewport(int x,int y,int w,int h,int depth) ;

    static Matrix44 zoom(float factor) ;
    static void zoom(Matrix44& mat, float factor) ;
    static Matrix44 tranlation(Vec3f v) ;
    static void translation(Matrix44& matrix,Vec3f v) ;

    static Matrix44 rotation_x(float cosangle,float sinangle) ;
    static Matrix44 rotation_y(float cosangle,float sinangle) ;
    static Matrix44 rotation_z(float cosangle,float sinangle) ;
    static Matrix44 projection(float camera) ;



    static  Matrix44 lookAt(Vec3f eye,Vec3f center ,Vec3f up);
    static Matrix44 lookAtMatrix( Vec3f const & eye, Vec3f const & center, Vec3f const & up) ;
    static Matrix44 rotate(Matrix44 const & m,float angle,Vec3f const & v) ;


    static Matrix44 perspective(float fovy, float aspect, float zNear, float zFar) ;
};


#endif //TINYRENDER_MANIPULATION_H
