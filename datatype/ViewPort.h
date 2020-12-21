//
// Created by lidan on 21/12/2020.
//

#ifndef NEWCPURENDER_VIEWPORT_H
#define NEWCPURENDER_VIEWPORT_H


#include "Vec.h"
#include "camera.h"
#include "manipulation.h"

class ViewPort{
private:
    Matrix44 ViewPortMatrix ;

public:
    ViewPort(int height, int width)
    {
        ViewPortMatrix = manipulation::viewport(width/8,height/8,width*3/4,height*3/4,255) ;
    }

    void change(std::vector<Matrix41>& verts)
    {
        for(int i = 0 ; i < verts.size(); i++)
        {
            verts[i] = ViewPortMatrix*verts[i] ;
        }
    }
    Matrix44 getMatrix() { return ViewPortMatrix ; }

};


#endif //NEWCPURENDER_VIEWPORT_H
