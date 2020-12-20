//
// Created by 李源 on 2020-12-18.
//

#ifndef NEWCPURENDER_PROJECT_H
#define NEWCPURENDER_PROJECT_H

#include "Vec.h"
#include "camera.h"
#include "manipulation.h"

class Project{
private:
    Matrix44 projectMatrix ;

public:
    Project(camera& ca,int HEIGHT, int WIDTH, float near, float far )
    {
        projectMatrix = manipulation::perspective(radians(ca.getZoom()), float(WIDTH)/float(HEIGHT), 0.1f, 1000.0f) ;
    }

    void change(std::vector<Matrix41>& verts)
    {
        for(int i = 0 ; i < verts.size(); i++)
        {
            verts[i] = projectMatrix*verts[i] ;
        }
    }
    Matrix44 getMatrix() { return projectMatrix ; }

};

#endif //NEWCPURENDER_PROJECT_H
