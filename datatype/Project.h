//
// Created by 李源 on 2020-12-18.
//

#ifndef NEWCPURENDER_PROJECT_H
#define NEWCPURENDER_PROJECT_H

#include "Vec.h"

class Project{
private:
    Matrix44 projectMatrix ;

public:
    Project()
    {
        projectMatrix[0][0] = 1.0f ;
        projectMatrix[1][1] = 1.0f ;
        projectMatrix[2][2] = 1.0f ;
        projectMatrix[3][3] = 1.0f ;
    }

    void change(std::vector<Matrix41>& verts)
    {
        for(int i = 0 ; i < verts.size(); i++)
        {
            verts[i] = projectMatrix*verts[i] ;
        }
    }

};

#endif //NEWCPURENDER_PROJECT_H
