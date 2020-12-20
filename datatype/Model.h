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
        modelMatrix = Matrix44::indentity() ;
    }

    Model(Vec3f center)
    {
        modelMatrix = Matrix44::indentity() ;
        Vec3f toCenterT = Vec3f(0.0) - center ;
        manipulation::translation(modelMatrix,toCenterT) ;
    }

    Matrix44 getMatrix() { return modelMatrix ; }

    void change(std::vector<Matrix41>& verts)
    {
        for(int i = 0 ; i < verts.size(); i++)
        {
            verts[i] = modelMatrix*verts[i] ;
        }
    }
};
#endif //NEWCPURENDER_MODEL_H
