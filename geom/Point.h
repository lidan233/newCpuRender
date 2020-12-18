//
// Created by 李源 on 2020-12-18.
//

#ifndef NEWCPURENDER_POINT_H
#define NEWCPURENDER_POINT_H


#include "Vec.h"

class Point {
private:
    double x_ ;
    double y_ ;
    char* data ;

public:
    Point(double x ,double y):x_(x),y_(y){}
    Point(const Point& ano):x_(ano.x_),y_(ano.y_){}
    Point(const Vec3f& vec):x_(vec[0]),y_(vec[1]){}
    Point* CopyPoint(Point& point){return new Point(point);}

    double getX(){return x_ ;}
    double getY(){return y_ ;}
};

#endif //NEWCPURENDER_POINT_H
