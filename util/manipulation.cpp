//
// Created by 李源 on 2019-11-29.
//

#include "manipulation.h"


Vec3f manipulation::m2v(Matrix41 m)
{
    return Vec3f(m[0][0]/m[3][0],m[1][0]/m[3][0],m[2][0]/m[3][0]) ;
}


Matrix41& manipulation::v2m(Vec3f v)
{
    Matrix41 m ;
    m[0][0] = v[0] ;
    m[1][0] = v[1] ;
    m[2][0] = v[2] ;
    m[3][0] = 1.f ;

    return m ;
}

Matrix44& manipulation::viewport(int x,int y,int w,int h,int depth)
{
    Matrix44 m = Matrix44::indentity() ;// 暂时默认是4 后期通过#define在data.h中解决

    m[0][3] = x+w/2.f ;
    m[1][3] = y+h/2.f ;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f ;
    m[1][1] = h/2.f ;
    m[2][2] = depth/2.f ;
    return m ;
}


Matrix44& manipulation::zoom(float factor)
{
    Matrix44 Z = Matrix44::indentity() ;
    Z[0][0] =  Z[1][1] =  Z[2][2] = factor  ;
    return Z ;
}


Matrix44& manipulation::tranlation(Vec3f v)
{
    Matrix44 Tr = Matrix44::indentity();
    Tr[0][3] = v.x ;
    Tr[1][3] = v.y ;
    Tr[2][3] = v.z ;
    return Tr  ;
}

//逆时针旋转
Matrix44& manipulation::rotation_x(float cosangle,float sinangle)
{
    Matrix44 R = Matrix44::indentity() ;
    R[1][1] = R[2][2] = cosangle ;
    R[1][2] = -sinangle ;
    R[2][1] = sinangle ;
    return R ;
}


Matrix44& manipulation::rotation_y(float cosangle,float sinangle)
{
    Matrix44 R = Matrix44::indentity() ;
    R[0][0] = R[2][2] = cosangle ;
    R[0][2] = -sinangle ;
    R[2][0] = sinangle ;
    return R ;
}

Matrix44& manipulation::rotation_z(float cosangle,float sinangle)
{
    Matrix44 R = Matrix44::indentity() ;
    R[0][0] = R[1][1] = cosangle ;
    R[0][1] = -sinangle ;
    R[1][0] = sinangle ;
}

Matrix44& manipulation::projection(float camera){
    Matrix44 R = Matrix44::indentity() ;
    R[3][2] = -1.f/float(camera) ;
    return R ;

}

Matrix44& manipulation::lookAt(Vec3f eye,Vec3f center ,Vec3f up)
{
    //左手系 右手系可以改为center-eye 其他不用动
    Vec3f z = (eye-center).normalize() ;
    Vec3f x = cross(up,z).normalize() ;
    Vec3f y = cross(z,x).normalize() ;

    Matrix44 minv = Matrix44::indentity() ;
    for(int i = 0; i< 3;i++)
    {
        minv[0][i] = x[i] ;
        minv[1][i] = y[i] ;
        minv[2][i] = z[i] ;
        //平移操作
        minv[i][3]  =  -center[i] ;
    }

    return minv ;
}


