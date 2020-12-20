//
// Created by 李源 on 2020-12-18.
//

#ifndef NEWCPURENDER_VIEW_H
#define NEWCPURENDER_VIEW_H

#include "Vec.h"
#include "camera.h"


class View{
private:
    Matrix44 viewMatrix ;

public:
    View(camera& ca)
    {
        viewMatrix = ca.GetViewMatrix() ;
    }


    void change(std::vector<Matrix41>& verts)
    {
        for(int i = 0 ; i < verts.size(); i++)
        {
            verts[i] = viewMatrix*verts[i] ;
        }
    }
    Matrix44 getMatrix() { return viewMatrix ; }
};

#endif //NEWCPURENDER_VIEW_H
