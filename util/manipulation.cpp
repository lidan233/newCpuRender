//
// Created by 李源 on 2019-11-29.
//

#include "manipulation.h"


Vec3f manipulation::m2v(Matrix41 m)
{
    return Vec3f(m[0][0]/m[3][0],m[1][0]/m[3][0],m[2][0]/m[3][0]) ;
}


Matrix41 manipulation::v2m(Vec3f v)
{
    Matrix41 m ;
    m[0][0] = v[0] ;
    m[1][0] = v[1] ;
    m[2][0] = v[2] ;
    m[3][0] = 1.f ;

    return m ;
}

Matrix44 manipulation::viewport(int x,int y,int w,int h,int depth)
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


Matrix44 manipulation::zoom(float factor)
{
    Matrix44 Z = Matrix44::indentity() ;
    Z[0][0] =  Z[1][1] =  Z[2][2] = factor  ;
    return Z ;
}

void manipulation::zoom(Matrix44& mat, float factor)
{
    mat[0][0] *= factor ;
    mat[1][1] *= factor ;
    mat[2][2] *= factor ;

}

Matrix44 manipulation::tranlation(Vec3f v)
{
    Matrix44 Tr = Matrix44::indentity();
    Tr[0][3] = v.x ;
    Tr[1][3] = v.y ;
    Tr[2][3] = v.z ;
    return Tr  ;
}

void manipulation::translation(Matrix44& mat,Vec3f v)
{
    mat[0][3] += v.x ;
    mat[1][3] += v.y ;
    mat[2][3] += v.z ;
}





//逆时针旋转
Matrix44 manipulation::rotation_x(float cosangle,float sinangle)
{
    Matrix44 R = Matrix44::indentity() ;
    R[1][1] = R[2][2] = cosangle ;
    R[1][2] = -sinangle ;
    R[2][1] = sinangle ;
    return R ;
}


Matrix44 manipulation::rotation_y(float cosangle,float sinangle)
{
    Matrix44 R = Matrix44::indentity() ;
    R[0][0] = R[2][2] = cosangle ;
    R[0][2] = -sinangle ;
    R[2][0] = sinangle ;
    return R ;
}

Matrix44 manipulation::rotation_z(float cosangle,float sinangle)
{
    Matrix44 R = Matrix44::indentity() ;
    R[0][0] = R[1][1] = cosangle ;
    R[0][1] = -sinangle ;
    R[1][0] = sinangle ;
    return R ;
}

Matrix44 manipulation::projection(float camera){
    Matrix44 R = Matrix44::indentity() ;
    R[3][2] = -1.f/float(camera) ;
    return R ;
}

Matrix44 manipulation::lookAt(Vec3f eye,Vec3f center ,Vec3f up)
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

Matrix44 manipulation::lookAtMatrix( Vec3f const & eye, Vec3f const & center, Vec3f const & up)
{
    Vec3f const f((center - eye).normalize());
    Vec3f const s((cross(f, up)).normalize());
    Vec3f const u(cross(s, f));

    Matrix44 res = Matrix44::indentity() ;
    res[0][0] = s.x;
    res[1][0] = s.y;
    res[2][0] = s.z;
    res[0][1] = u.x;
    res[1][1] = u.y;
    res[2][1] = u.z;
    res[0][2] =-f.x;
    res[1][2] =-f.y;
    res[2][2] =-f.z;
    res[3][0] =-(s * eye);
    res[3][1] =-(u * eye);
    res[3][2] =(f * eye);
    return res.transpose();
}

Matrix44 manipulation::rotate(Matrix44 const & m,float angle,Vec3f const & v)
{
    float const a = angle;
    float const c = cos(a);
    float const s = sin(a);


    Vec3f axis(v.normalize());
    Vec3f temp((float(1) - c) * axis);

    Matrix44 Rotate ;
    Rotate[0][0] = c + temp[0] * axis[0];
    Rotate[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
    Rotate[0][2] = 0 + temp[0] * axis[2] - s * axis[1];

    Rotate[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
    Rotate[1][1] = c + temp[1] * axis[1];
    Rotate[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

    Rotate[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
    Rotate[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
    Rotate[2][2] = c + temp[2] * axis[2];

    Matrix44 Result;
    Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
    Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
    Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
    Result[3] = m[3];
    return Result.transpose();
}

Matrix44 manipulation::perspective(float fovy, float aspect, float zNear, float zFar)
{
    assert(abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0));

    float const tanHalfFovy = tan(fovy / static_cast<float>(2));

    Matrix44 Result;
    Result[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
    Result[1][1] = static_cast<float>(1) / (tanHalfFovy);
    Result[2][2] = - (zFar + zNear) / (zFar - zNear);
    Result[2][3] = - static_cast<float>(1);
    Result[3][2] = - (static_cast<float>(2) * zFar * zNear) / (zFar - zNear);
    return Result.transpose();
}


