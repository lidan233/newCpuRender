//
// Created by 李源 on 2020-12-18.
//

#ifndef NEWCPURENDER_VIEW_H
#define NEWCPURENDER_VIEW_H

#include "Vec.h"

class View{
private:
    Matrix44 viewMatrix ;

public:
    View()
    {
        viewMatrix[0][0] = 1.0f ;
        viewMatrix[1][1] = 1.0f ;
        viewMatrix[2][2] = 1.0f ;
        viewMatrix[3][3] = 1.0f ;
    }
    void change(std::vector<Matrix41>& verts)
    {
        for(int i = 0 ; i < verts.size(); i++)
        {
            verts[i] = viewMatrix*verts[i] ;
        }
    }
};

#endif //NEWCPURENDER_VIEW_H
