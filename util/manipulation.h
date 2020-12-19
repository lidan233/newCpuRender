//
// Created by 李源 on 2019-11-29.
//

#ifndef TINYRENDER_MANIPULATION_H
#define TINYRENDER_MANIPULATION_H


#include "Vec.h"

class manipulation {

public:
    static Vec3f m2v(Matrix41 m) ;
    static Matrix41& v2m(Vec3f v) ;
    static Matrix44& viewport(int x,int y,int w,int h,int depth) ;

    static Matrix44& zoom(float factor) ;
    static Matrix44& zoom(Matrix44& mat, float factor) ;
    static Matrix44& tranlation(Vec3f v) ;
    static Matrix44& translation(Matrix44& mat,Vec3f v) ;

//逆时针旋转
    static Matrix44& rotation_x(float cosangle,float sinangle) ;

    static Matrix44& rotation_y(float cosangle,float sinangle) ;

    static Matrix44& rotation_z(float cosangle,float sinangle) ;

    static Matrix44& projection(float camera) ;
    static  Matrix44& lookAt(Vec3f eye,Vec3f center ,Vec3f up);


};


#endif //TINYRENDER_MANIPULATION_H
