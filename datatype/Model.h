//
// Created by 李源 on 2020-12-18.
//

#ifndef NEWCPURENDER_MODEL_H
#define NEWCPURENDER_MODEL_H

#include "Vec.h"
#include "manipulation.h"

#include <vector>

class Model{
private:
    Matrix44 modelMatrix ;

public:
    Model()
    {
        modelMatrix[0][0] = 1.0f ;
        modelMatrix[1][1] = 1.0f ;
        modelMatrix[2][2] = 1.0f ;
        modelMatrix[3][3] = 1.0f ;
    }

    Model(Vec3f center)
    {

    }

    void change(std::vector<Matrix41>& verts)
    {
        for(int i = 0 ; i < verts.size(); i++)
        {
            verts[i] = modelMatrix*verts[i] ;
        }
    }
};
#endif //NEWCPURENDER_MODEL_H
